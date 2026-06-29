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
#define TIMEOUT 30

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

bool Cluster::isCGIPipe(int pipeFd) const
{
    return _cgiStates.find(pipeFd) != _cgiStates.end();
}

bool Cluster::hasPendingCGI(int clientFd) const
{
    std::map<int, CGIState>::const_iterator it;
    for (it = _cgiStates.begin(); it != _cgiStates.end(); ++it)
    {
        if (it->second.clientFd == clientFd)
            return true;
    }
    return false;
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

			if (isCGIPipe(currentFd))
				checkCGITimeout(currentFd);
			if ((revents & POLLOUT) && !findServer(currentFd) && !isCGIPipe(currentFd))
				handleClientWrite(currentFd, i);
			else if (revents & (POLLIN | POLLHUP | POLLERR))
			{
				if (findServer(currentFd))
        				acceptClient(currentFd);
    				else if (isCGIPipe(currentFd))
        				handleCGI(currentFd);
    				else
        				handleClientData(currentFd, i);
			}

			if (isCGIPipe(currentFd))
    				checkCGITimeout(currentFd);
		}
		if (_needsCompaction) {
			compactPollFds();
	}
	checkInactiveClients();
	}
}

std::string buildError(int code);
std::string getFileContent(std::string path);
static std::string getErrorBody(int error)
{
    const std::string& path = buildError(error);
    std::string content = "";

    if (HttpHandler::isFileInError(R_OK, path) == 0)
        content = getFileContent(path);

		return content;
}

void Cluster::checkCGITimeout(int pipeFd)
{
    std::map<int, CGIState>::iterator it = _cgiStates.find(pipeFd);
    if (it == _cgiStates.end())
        return;

    CGIState &cgi = it->second;

    if (std::time(NULL) - cgi.start <= TIMEOUT)
        return;

    kill(cgi.cgiPid, SIGKILL);
    waitpid(cgi.cgiPid, NULL, 0);
    close(pipeFd);

    for (size_t i = 0; i < _fds.size(); i++) {
        if (_fds[i].fd == pipeFd) {
            _fds[i].fd = -1;
            break;
        }
    }
    _needsCompaction = true;

    std::map<int, Client>::iterator clientIt = _clientsFds.find(cgi.clientFd);
    if (clientIt != _clientsFds.end()) {
        Response response;
        response.setVersion(cgi.version);
        response.setResponseData(504, "Gateway Timeout", getErrorBody(504));
        queueResponse(clientIt->second, cgi.clientFd, response);
    }

    _cgiStates.erase(it);
}

void Cluster::handleCGI(int pipeFd)
{
    std::map<int, CGIState>::iterator it = _cgiStates.find(pipeFd);
    if (it == _cgiStates.end())
        return;

    CGIState &cgi = it->second;

    char    buf[65536];
    ssize_t n = read(pipeFd, buf, sizeof(buf));

    if (n > 0) {
        cgi.output.append(buf, n);
        return;
    }

    if (n == -1 && errno == EAGAIN)
        return;

    close(pipeFd);

    for (size_t i = 0; i < _fds.size(); i++) {
        if (_fds[i].fd == pipeFd) {
            _fds[i].fd = -1;
            break;
        }
    }
    _needsCompaction = true;

    int status = 0;
    waitpid(cgi.cgiPid, &status, 0);

    std::map<int, Client>::iterator clientIt = _clientsFds.find(cgi.clientFd);
    if (clientIt != _clientsFds.end()) {
        Response response;
        response.setVersion(cgi.version);
        if (n == 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            int    statusCode = 200;
            std::string statusMsg  = "OK";
            std::string body;

            size_t sep = cgi.output.find("\r\n\r\n");
            if (sep == std::string::npos)
                sep = cgi.output.find("\n\n");

            if (sep != std::string::npos)
            {
                size_t sepLen = (cgi.output[sep] == '\r') ? 4 : 2;
                std::string cgiHeaders = cgi.output.substr(0, sep);
                body = cgi.output.substr(sep + sepLen);

                size_t statusPos = cgiHeaders.find("Status:");
                if (statusPos != std::string::npos)
                {
                    size_t eol = cgiHeaders.find('\n', statusPos);
                    std::string sl = cgiHeaders.substr(statusPos + 7,
                                        eol == std::string::npos ? std::string::npos : eol - statusPos - 7);
                    size_t s = sl.find_first_not_of(" \t");
                    if (s != std::string::npos)
                    {
                        statusCode = std::atoi(sl.c_str() + s);
                        size_t sp = sl.find(' ', s);
                        if (sp != std::string::npos)
                        {
                            statusMsg = sl.substr(sp + 1);
                            size_t trim = statusMsg.find_last_not_of(" \t\r\n");
                            if (trim != std::string::npos)
                                statusMsg = statusMsg.substr(0, trim + 1);
                        }
                    }
                }

                size_t ctPos = cgiHeaders.find("Content-Type:");
                if (ctPos != std::string::npos)
                {
                    size_t eol = cgiHeaders.find('\n', ctPos);
                    std::string ct = cgiHeaders.substr(ctPos + 13,
                                        eol == std::string::npos ? std::string::npos : eol - ctPos - 13);
                    size_t s = ct.find_first_not_of(" \t");
                    if (s != std::string::npos)
                    {
                        ct = ct.substr(s);
                        size_t trim = ct.find_last_not_of(" \t\r\n");
                        if (trim != std::string::npos)
                            ct = ct.substr(0, trim + 1);
                        response.setHeader("Content-Type", ct);
                    }
                }
            }
            else
            {
                body = cgi.output;
            }

            response.setResponseData(statusCode, statusMsg, body);
        }
        else
            response.setResponseData(500, "Internal Server Error", getErrorBody(500));
        queueResponse(clientIt->second, cgi.clientFd, response);
    }

    _cgiStates.erase(it);
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

	char buffer[65536] = {0};
	int  bytes_read    = recv(clientFd, buffer, sizeof(buffer), 0);

	if (bytes_read > 0) {
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
		if (hasPendingCGI(clientFd)) {
			// Client half-closed (sent FIN); CGI still running.
			// Remove from poll to avoid busy-loop on POLLHUP, keep fd open for response.
			_fds[pollIndex].fd = -1;
			_needsCompaction = true;
		} else {
			Client& cl = _clientsFds[clientFd];
			if (cl.hasDataToSend()) {
				// Half-closed: client done sending, but we still have a response queued.
				// Attempt to send it now; poll POLLOUT will handle remaining chunks.
				handleClientWrite(clientFd, pollIndex);
			} else {
				std::ostringstream oss;
				oss << "Client closed connection clientFd=" << clientFd;
				print_msg(oss.str(), DISC);
				disconnectClient(clientFd, pollIndex);
			}
		}
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
	Server* server = findServer(client.getServerFd());
	if (!server) {
		print_msg("Server not found for client", ERR);
		disconnectClient(clientFd, pollIndex);
		return;
	}
	Request      request;
	Response     response;
	ServerParser serverConfig;

	if (request.parse(client.getBuffer()) != 0) {
		response.setVersion("HTTP/1.1");
		ErrorHandler(400, request, serverConfig, response);
		queueResponse(client, clientFd, response);
		return;
	}
	
	if (request.getVersion() != "HTTP/1.0" && request.getVersion() != "HTTP/1.1" && request.getVersion() != "HTTP/0.9") {
		response.setVersion("HTTP/1.1");
		ErrorHandler(505, request, serverConfig, response);
		queueResponse(client, clientFd, response);
		return;
	}
	
	response.setVersion(request.getVersion());
	std::string lenStr = request.getHeader("Content-Length");
	size_t contentLength = std::strtoul(lenStr.c_str(), NULL, 10);
	bool bodyTooLarge = false;

	if (!request.getIsChunked())
		bodyTooLarge = contentLength > client.getMaxBodySize();

	if (client.isBodyTooLarge() || bodyTooLarge)
	{
			ErrorHandler(413, request, serverConfig, response);
			queueResponse(client, clientFd, response);
			return;
	}

	serverConfig = Router::findMatchingServer(request, server->getConfigs());

	const LocationParser* location = Router::findMatchingLocation(request, serverConfig);

	// For chunked requests, use actual decoded body size; for others use Content-Length
	size_t effectiveBodySize = request.getIsChunked() ? request.getBody().size() : contentLength;
	if (location && location->getMaxBodySize() > 0 && effectiveBodySize > location->getMaxBodySize())
	{
		ErrorHandler(413, request, serverConfig, response);
		queueResponse(client, clientFd, response);
		return;
	}

	HttpHandler* const handler = HandlerFactory::create(request, location, serverConfig);

	if (handler) {
		handler->handleRequest(response);
		CGIState cgi = response.getCGIState();
		if (cgi.isCgi == true)
		{
			cgi.clientFd = clientFd;
			cgi.start = std::time(NULL);
			cgi.version = response.getVersion();
			_cgiStates[cgi.pipeFd] = cgi;

			struct pollfd pfd;
			pfd.fd      = cgi.pipeFd;
			pfd.events  = POLLIN | POLLHUP;
			pfd.revents = 0;
			_fds.push_back(pfd);
		}
		else
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

	if (!client.hasDataToSend())
		return;

	ssize_t sent = send(clientFd,
						client.getWritePtr(),
						client.getWriteRemaining(),
						0);

	if (sent > 0) {
		client.advanceWriteOffset(static_cast<size_t>(sent));
		client.updateActivity();
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

	bool found = false;
	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd == clientFd) {
			_fds[i].events = POLLOUT;
			found = true;
			break;
		}
	}
	if (!found) {
		struct pollfd pfd;
		pfd.fd      = clientFd;
		pfd.events  = POLLOUT;
		pfd.revents = 0;
		_fds.push_back(pfd);
	}

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

	if (isCGIPipe(fd))
		return false;

	// Don't disconnect if there's a queued response to send (half-closed TCP)
	std::map<int, Client>::iterator cit = _clientsFds.find(fd);
	if (cit != _clientsFds.end() && cit->second.hasDataToSend())
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
