/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datienza <datienza@student.42barcelo>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 23:08:40 by datienza          #+#    #+#             */
/*   Updated: 2026/04/21 23:08:45 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Cluster.hpp>

Cluster::Cluster(const std::map<int, std::vector<ServerParser> >& configs)
	: _needsCompaction(false) {

	init(configs);
}

Cluster::Cluster(const Cluster& other) {

	*this = other;
}

Cluster::~Cluster() {

	for (size_t i = 0; i < _servers.size(); i++) {
		delete _servers[i];
	}

	this->_servers.clear();

	this->_fds.clear();
}

Cluster& Cluster::operator=(const Cluster& other) {

	if (this != &other) {
		_servers    = other._servers;
		_fds        = other._fds;
		_clientsFds = other._clientsFds;
		_needsCompaction = other._needsCompaction;
	}
	return *this;
}

void Cluster::init(const std::map<int, std::vector<ServerParser> >& configs) {

	signal(SIGPIPE, SIG_IGN);

	std::map<int, std::vector<ServerParser> >::const_iterator it;
	for (it = configs.begin(); it != configs.end(); ++it) {
		Server* newServer = new Server(it->first, it->second);

		if (!newServer->init()) {
			delete newServer;
			continue;
		}

		struct pollfd pfd;
		pfd.fd      = newServer->getFd();
		pfd.events  = POLLIN;
		pfd.revents = 0;
		this->_fds.push_back(pfd);

		this->_servers.push_back(newServer);
	}

	std::ostringstream oss;
	oss << "Total server sockets: " << _servers.size();
	print_msg(oss.str(), DEBUG);
}

Server* Cluster::findServer(int fd) {

	for (size_t i = 0; i < _servers.size(); i++) {
		if (this->_servers[i]->getFd() == fd)
			return _servers[i];
	}
	return NULL;
}

void Cluster::run() {

	if (_servers.empty()) {
		print_msg("No server socket could be initialized", FATAL);
		return;
	}

	std::ostringstream start;
	start << "Server running with " << _servers.size() << " sockets";
	print_msg(start.str(), START);

	while (true) {
		int poll_count = poll(&_fds[0], _fds.size(), 5000);
		if (poll_count < 0) {
			print_msg("poll() failed", FATAL);
			break;
		}

		size_t currentSize = _fds.size();

		for (size_t i = 0; i < currentSize; i++) {

			int currentFd = _fds[i].fd;
			if (currentFd < 0)
				continue;

			short revents = _fds[i].revents;

			if (handleClientError(currentFd, i, revents))
				continue;

			if (revents & POLLIN) {
				std::ostringstream oss;
				oss << "POLLIN fd=" << currentFd << " index=" << i;
				print_msg(oss.str(), DEBUG);

				if (findServer(currentFd))
					acceptClient(currentFd);
				else
					handleClientData(currentFd, i);
			}
			else if (revents & POLLOUT) {
				std::ostringstream oss;
				oss << "POLLOUT fd=" << currentFd << " index=" << i;
				print_msg(oss.str(), DEBUG);

				handleClientWrite(currentFd, i);
			}
		}
		if (_needsCompaction) {
			compactPollFds();
	}
	checkInactiveClients();
	}
}

void Cluster::acceptClient(int serverFd) {

	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int clientFd = accept(serverFd,
						 (struct sockaddr*)&clientAddr,
						 &clientLen);
	if (clientFd < 0) {
		std::ostringstream oss;
		oss << "accept() failed serverFd=" << serverFd;
		print_msg(oss.str(), ERR);
		return;
	}

	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	_clientsFds[clientFd] = Client(clientFd, serverFd);

	Server* server = findServer(serverFd);
	if (server && !server->getConfigs().empty()) {
		_clientsFds[clientFd].setMaxBodySize(server->getConfigs()[0].getMaxBodySize());
	}

	struct pollfd pfd;
	pfd.fd      = clientFd;
	pfd.events  = POLLIN;
	pfd.revents = 0;
	_fds.push_back(pfd);

	std::ostringstream oss;
	oss << "Client connected clientFd=" << clientFd
		<< " serverFd=" << serverFd
		<< " total_clients=" << _clientsFds.size();
	print_msg(oss.str(), CONN);
}

void Cluster::handleClientData(int clientFd, size_t pollIndex) {

	char buffer[10000] = {0};
	int  bytes_read    = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_read > 0) {
		std::ostringstream oss;
		oss << "recv() clientFd=" << clientFd
			<< " bytes=" << bytes_read;
		print_msg(oss.str(), DEBUG);

		Client& client = _clientsFds[clientFd];
		client.appendData(buffer, bytes_read);
		client.updateActivity();

		if (client.isRequestComplete()) {
			std::ostringstream req;
			req << "Request complete clientFd=" << clientFd;
			print_msg(req.str(), REQ);
			processHttpRequest(client, clientFd, pollIndex);
		}
	}
	else if (bytes_read == 0) {
		std::ostringstream oss;
		oss << "Client closed connection clientFd=" << clientFd;
		print_msg(oss.str(), DISC);
		disconnectClient(clientFd, pollIndex);
	}
	else {
		std::ostringstream oss;
		oss << "recv() returned -1 clientFd=" << clientFd
			<< " closing connection";
		print_msg(oss.str(), ERR);
		disconnectClient(clientFd, pollIndex);
	}
}

void Cluster::processHttpRequest(Client& client, int clientFd, size_t pollIndex) {

	std::ostringstream start;
	start << "Processing request clientFd=" << clientFd
		  << " serverFd=" << client.getServerFd();
	print_msg(start.str(), DEBUG);

	Server* server = findServer(client.getServerFd());
	if (!server) {
		print_msg("Server not found for client", ERR);
		disconnectClient(clientFd, pollIndex);
		return;
	}

	Request      request;
	Response     response;
	ServerParser serverConfig;

	if (client.isBodyTooLarge()) {
		ErrorHandler(413, request, serverConfig, response);
		queueResponse(client, clientFd, response);
		return;
	}

	if (request.parse(client.getBuffer()) != 0) {
		ErrorHandler(400, request, serverConfig, response);
		queueResponse(client, clientFd, response);
		return;
	}
	
	if (request.getVersion() != "HTTP/1.0" && request.getVersion() != "HTTP/1.1") {
		ErrorHandler(505, request, serverConfig, response);
		queueResponse(client, clientFd, response);
		return;
	}

	serverConfig = Router::findMatchingServer(request, server->getConfigs());

	const LocationParser* location = Router::findMatchingLocation(request, serverConfig);
	HttpHandler* const handler = HandlerFactory::create(request, location, serverConfig);

	if (handler) {
		handler->handleRequest(response);
		queueResponse(client, clientFd, response);
		delete handler;
	}
	else {
		std::ostringstream oss;
		oss << "Handler NULL clientFd=" << clientFd;
		print_msg(oss.str(), ERR);
		disconnectClient(clientFd, pollIndex);
	}
}

void Cluster::handleClientWrite(int clientFd, size_t pollIndex) {

	std::map<int, Client>::iterator it = _clientsFds.find(clientFd);
	if (it == _clientsFds.end()) {
		print_msg("Client not found in handleClientWrite", ERR);
		disconnectClient(clientFd, pollIndex);
		return;
	}

	Client& client = it->second;

	if (!client.hasDataToSend()) {
		std::ostringstream oss;
		oss << "POLLOUT fired but nothing to send clientFd=" << clientFd;
		print_msg(oss.str(), DEBUG);
		disconnectClient(clientFd, pollIndex);
		return;
	}

	ssize_t sent = send(clientFd,
						client.getWritePtr(),
						client.getWriteRemaining(),
						0);

	if (sent > 0) {
		client.advanceWriteOffset(static_cast<size_t>(sent));

		std::ostringstream oss;
		oss << "send() clientFd=" << clientFd
			<< " bytes=" << sent
			<< " remaining=" << client.getWriteRemaining();
		print_msg(oss.str(), DEBUG);

		if (client.isResponseFullySent()) {
			std::ostringstream done;
			done << "Response fully sent clientFd=" << clientFd;
			print_msg(done.str(), RES);
			disconnectClient(clientFd, pollIndex);
		}
	}
	else {
		std::ostringstream oss;
		oss << "send() returned " << sent
			<< " clientFd=" << clientFd
			<< " closing connection";
		print_msg(oss.str(), ERR);
		disconnectClient(clientFd, pollIndex);
	}
}

void Cluster::disconnectClient(int clientFd, size_t pollIndex) {

	close(clientFd);
	_fds[pollIndex].fd = -1;
	_clientsFds.erase(clientFd);
	_needsCompaction = true;

	std::ostringstream oss;
	oss << "Client disconnected clientFd=" << clientFd
	<< " _fds.size()=" << _fds.size();
	print_msg(oss.str(), DISC);
}

void Cluster::setPollEvents(int fd, short events) {

	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd == fd) {
			_fds[i].events = events;
			return;
		}
	}
}

void Cluster::queueResponse(Client& client, int clientFd, Response& response) {

	std::string msg = response.buildResponse();

	client.setResponse(msg);
	setPollEvents(clientFd, POLLOUT);

	std::ostringstream oss;
	oss << "Response queued clientFd=" << clientFd
		<< " bytes=" << msg.size();
	print_msg(oss.str(), RES);
}

bool Cluster::handleClientError(int fd, size_t pollIndex, short revents) {

	if (!(revents & (POLLERR | POLLHUP | POLLNVAL)))
		return false;

	if (findServer(fd))
		return false;

	std::ostringstream oss;
	oss << "POLL error/hangup fd=" << fd
		<< " revents=" << revents;
	print_msg(oss.str(), DISC);

	disconnectClient(fd, pollIndex);
	return true;
}

void Cluster::compactPollFds() {

	std::vector<struct pollfd> compacted;

	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd >= 0) {
			compacted.push_back(_fds[i]);
		}
	}

	_fds.swap(compacted);
	_needsCompaction = false;
}

void Cluster::checkInactiveClients() {

	time_t now    = time(NULL);
	int    timeout = 60;

	std::map<int, Client>::iterator it = _clientsFds.begin();
	while (it != _clientsFds.end()) {

		Client& client = it->second;

		if (now - client.getLastActivity() > timeout) {
			std::ostringstream oss;
			oss << "Client timeout clientFd=" << client.getFd();
			print_msg(oss.str(), DISC);

			for (size_t i = 0; i < _fds.size(); i++) {
				if (_fds[i].fd == client.getFd()) {
					disconnectClient(client.getFd(), i);
					break;
				}
			}

			it = _clientsFds.begin();
		}
		else {
			++it;
		}
	}
}
