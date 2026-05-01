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

//Orthodox Canonical Form

Cluster::Cluster(const std::map<int, std::vector<ServerConfig> >& servers) : _configs(servers) {
	print_msg("Initializing cluster", DEBUG);
	init();
}

Cluster::Cluster(const Cluster & other) : _configs(other._configs) {
	*this = other;
}


Cluster::~Cluster() {
    
    for (size_t i = 0; i < this->_fds.size(); ++i) {
        if (this->_fds[i].fd >= 0) {
            close(this->_fds[i].fd);
        //print_msg(); Puertos cerrados?
		}
    }
    this->_fds.clear();
}

Cluster & Cluster::operator=(const Cluster & other) {
	if (this != &other) {
		this->_fds = other._fds;
		this->_serverFds = other._serverFds;
		this->_clientsFds = other._clientsFds;
	}
	return *this;
}


void Cluster::init() {
	
	std::map<int, std::vector<ServerConfig> >::const_iterator it;

	for (it = _configs.begin(); it != _configs.end(); ++it) {
		int port = it->first;

		int serverFd = socket(AF_INET, SOCK_STREAM, 0);
		if (serverFd < 0) {
			print_msg("Failed to create socket", FATAL);
			return; 
		}

		fcntl(serverFd, F_SETFL, O_NONBLOCK); //socket no bloqueante

		int opt = 1;
		if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			print_msg("Failed to set SO_REUSEADDR", WARN);
		}
		
		struct sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr)); 
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
			std::cout << errno << std::endl;
			print_msg("Bind failed", FATAL); 
			close(serverFd);
			return;
		}

		if (listen(serverFd, SOMAXCONN) < 0) {
			print_msg("Listen failed", FATAL);
			close(serverFd);
			return;
		}

		this->_serverFds[serverFd] = it->second;

		struct pollfd pfd;
		pfd.fd = serverFd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		
		this->_fds.push_back(pfd);

		std::ostringstream oss;
		oss << "Listening on port " << port;
		print_msg(oss.str(), SUCCESS);
	}
}

void Cluster::acceptClient(int serverFd) {
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);

	if (clientFd >= 0) {
		print_msg("Client accepted", SUCCESS);
		
		fcntl(clientFd, F_SETFL, O_NONBLOCK);
		
		this->_clientsFds[clientFd] = Client(clientFd, serverFd);

		struct pollfd pfd;
		pfd.fd = clientFd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		this->_fds.push_back(pfd);
	}
}

void Cluster::run() {

	while (true) {
		int poll_count = poll(&this->_fds[0], this->_fds.size(), -1);
		if (poll_count < 0) {
			print_msg("Poll error", FATAL);
			break;
		}

		for (size_t i = 0; i < this->_fds.size(); ++i) {
			
			if (!(this->_fds[i].revents & POLLIN)) 
				continue; 
				
			int currentFd = this->_fds[i].fd;

			//SERVIDOR
			if (this->_serverFds.find(currentFd) != this->_serverFds.end()) {
				this->acceptClient(currentFd);
			}
			// CLIENTE
			else {
				char buffer[10000] = {0}; //TODO SIZE limit
				int bytes_read = recv(currentFd, buffer, sizeof(buffer) - 1, 0);

				if (bytes_read > 0) {

					this->_clientsFds[currentFd].appendData(buffer, bytes_read);

					if (this->_clientsFds[currentFd].isHeaderComplete()) {
						std::cout << "\n PETICION RECIBIDA \n";

						if (_clientsFds.find(currentFd)->second.isHeaderComplete())
						{
    						Request request(_clientsFds.find(currentFd)->second.getBuffer());
    						Response response;

    						const ServerConfig& server = Router::findMatchingServer(request, _serverFds.find(_clientsFds.find(currentFd)->second.getServerFd())->second);
    						const Location* location = Router::findMatchingLocation(request, server);
							
    						HttpHandler* const handler = HandlerFactory::create(request, location, server, response);
							
    						//std::cout << "Request: " << buffer << std::endl;

    						if (handler) //If ErrorHandler should not enter
								{
									handler->handleRequest(response);
									std::string msg = response.buildResponse();
									std::cout << msg << std::endl;
									send(currentFd, msg.c_str(), msg.size(), 0);
								}
								else
									std::cout << "NULL" << std::endl;
						}

						close(currentFd);
						this->_fds[i].fd = -1;
						this->_clientsFds.erase(currentFd);
					}
				} 
				else if (bytes_read == 0) {

					close(currentFd);
					this->_fds[i].fd = -1;
					this->_clientsFds.erase(currentFd);
				}
			}
		}
	}
}