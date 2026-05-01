/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datienza <datienza@student.42barcelo>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 20:15:03 by datienza          #+#    #+#             */
/*   Updated: 2026/04/30 20:15:06 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

Server::Server() : _fd(-1), _port(0) {}

Server::Server(int port, const std::vector<ServerConfig>& configs)
	: _fd(-1), _port(port), _configs(configs) {}

Server::Server(const Server& other)
	: _fd(other._fd), _port(other._port), _configs(other._configs) {}

Server& Server::operator=(const Server& other) {
	
	if (this != &other) {

		this->_fd      = other._fd;
		this->_port    = other._port;
		this->_configs = other._configs;
	}
	return *this;
}

Server::~Server() {
	if (this->_fd > 0)
		close(_fd);
}


int	Server::getFd()      const { 
	
	return this->_fd; 
}

int	Server::getPort()    const { 

	return this->_port;
}


const std::vector<ServerConfig>& Server::getConfigs() const {

	return this->_configs;
}

bool Server::createSocket() {

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		print_msg("socket() failed", FATAL);
		return false;
	}

	std::ostringstream oss;
	oss << "Socket created fd=" << this->_fd << " port=" << this->_port;
	print_msg(oss.str(), DEBUG);
	return true;
}

bool Server::setOptions() {

	int opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::ostringstream oss;
		oss << "setsockopt() failed fd=" << this->_fd;
		print_msg(oss.str(), WARN);
	}

	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) {
		std::ostringstream oss;
		oss << "fcntl() failed fd=" << this->_fd;
		print_msg(oss.str(), FATAL);
		return false;
	}
	return true;
}

bool Server::bindListen() {

	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons(this->_port);

	if (bind(this->_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		std::ostringstream oss;
		oss << "bind() failed fd=" << this->_fd << " port=" << this->_port;
		print_msg(oss.str(), FATAL);
		return false;
	}

	if (listen(this->_fd, SOMAXCONN) < 0) {
		std::ostringstream oss;
		oss << "listen() failed fd=" << this->_fd << " port=" << this->_port;
		print_msg(oss.str(), FATAL);
		return false;
	}

	std::ostringstream oss;
	oss << "Listening on port=" << this->_port << " fd=" << this->_fd;
	print_msg(oss.str(), SUCCESS);
	return true;
}

bool Server::init() {
	
	if (!createSocket())
		return false;

	if (!setOptions()){ 
		close(this->_fd); 
		this->_fd = -1; 
		return false; 
	}

	if (!bindListen()){ 
		close(_fd); 
		_fd = -1; 
		return false;
	}

	return true;
}
