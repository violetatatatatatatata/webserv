/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datienza <datienza@student.42barcelo>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 23:18:40 by datienza          #+#    #+#             */
/*   Updated: 2026/04/21 23:18:42 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _requestBuffer("") {}

Client::Client() : _fd(-1), _requestBuffer("") {}

Client::Client(const Client& other) {
	
	*this = other;
}

Client& Client::operator=(const Client& other) {
	
	if (this != &other) {
		this->_fd = other._fd;
		this->_requestBuffer = other._requestBuffer;
	}
	return *this;
}

Client::~Client() {}


int Client::getFd() const { return _fd; }

const std::string& Client::getBuffer() const { return _requestBuffer;}

void Client::appendData(const char* buffer, ssize_t bytesRead) {
	
	if (bytesRead > 0) {
		this->_requestBuffer.append(buffer, bytesRead);
	}
}

bool Client::isHeaderComplete() const {
	
if (this->_requestBuffer.find("\r\n\r\n") != std::string::npos) {
		return true;
	} else
		return false;
}