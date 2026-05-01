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

#include <Webserv.hpp>

Cluster::Cluster(const std::map<int, std::vector<ServerConfig> >& configs) {

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
	}
	return *this;
}

void Cluster::init(const std::map<int, std::vector<ServerConfig> >& configs) {

	std::map<int, std::vector<ServerConfig> >::const_iterator it;
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

	std::ostringstream start;
	start << "Server running with " << _servers.size() << " sockets";
	print_msg(start.str(), START);

	while (true) {
		int poll_count = poll(&_fds[0], _fds.size(), -1);
		if (poll_count < 0) {
			std::ostringstream oss;
			oss << "poll() failed errno=" << errno;
			print_msg(oss.str(), FATAL);
			break;
		}

		size_t currentSize = _fds.size();

		for (size_t i = 0; i < currentSize; i++) {
			if (!(_fds[i].revents & POLLIN))
				continue;

			int currentFd = _fds[i].fd;

			std::ostringstream oss;
			oss << "Activity fd=" << currentFd << " index=" << i;
			print_msg(oss.str(), DEBUG);

			if (findServer(currentFd))
				acceptClient(currentFd);
			else
				handleClientData(currentFd, i);
		}
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

		if (client.isHeaderComplete()) {
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
		oss << "recv() error clientFd=" << clientFd
			<< " errno=" << errno;
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

	Request  request(client.getBuffer());
	Response response;

	const ServerConfig& serverConfig = Router::findMatchingServer(
		request, server->getConfigs());
	const Location* location = Router::findMatchingLocation(
		request, serverConfig);

	HttpHandler* const handler = HandlerFactory::create(
		request, location, serverConfig, response);

	if (handler) {
		handler->handleRequest(response);
		std::string msg = response.buildResponse();

		ssize_t sent = send(clientFd, msg.c_str(), msg.size(), 0);

		std::ostringstream oss;
		oss << "Response sent clientFd=" << clientFd
			<< " bytes=" << sent;
		print_msg(oss.str(), RES);

		delete handler;
	}
	else {
		std::ostringstream oss;
		oss << "Handler NULL clientFd=" << clientFd;
		print_msg(oss.str(), ERR);
	}

	disconnectClient(clientFd, pollIndex);
}


void Cluster::disconnectClient(int clientFd, size_t pollIndex) {
	
	close(clientFd);
	_fds[pollIndex].fd = -1;
	_clientsFds.erase(clientFd);

	std::ostringstream oss;
	oss << "Client disconnected clientFd=" << clientFd;
	print_msg(oss.str(), DISC);
}