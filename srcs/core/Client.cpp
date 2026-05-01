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

Client::Client()
	: _fd(-1), _serverFd(-1),
	  _requestBuffer(""),
	  _responseBuffer(""), _responseOffset(0) {}

Client::Client(int fd, int serverFd)
	: _fd(fd), _serverFd(serverFd),
	  _requestBuffer(""),
	  _responseBuffer(""), _responseOffset(0) {}

Client::Client(const Client& other) {

	*this = other;
}

Client& Client::operator=(const Client& other) {

	if (this != &other) {
		this->_fd             = other._fd;
		this->_serverFd       = other._serverFd;
		this->_requestBuffer  = other._requestBuffer;
		this->_responseBuffer = other._responseBuffer;
		this->_responseOffset = other._responseOffset;
	}
	return *this;
}

Client::~Client() {}

int Client::getFd() const { return _fd; }

int Client::getServerFd() const { return _serverFd; }

const std::string& Client::getBuffer() const { return _requestBuffer; }

void Client::appendData(const char* buffer, ssize_t bytesRead) {

	if (bytesRead > 0) {
		this->_requestBuffer.append(buffer, bytesRead);
	}
}

bool Client::isHeaderComplete() const {

	if (this->_requestBuffer.find("\r\n\r\n") != std::string::npos) {
		return true;
	}
	return false;
}

void Client::setResponse(const std::string& response) {

	this->_responseBuffer = response;
	this->_responseOffset = 0;
}

bool Client::hasDataToSend() const {

	return this->_responseOffset < this->_responseBuffer.size();
}

const char* Client::getWritePtr() const {

	return this->_responseBuffer.c_str() + this->_responseOffset;
}

size_t Client::getWriteRemaining() const {

	return this->_responseBuffer.size() - this->_responseOffset;
}

void Client::advanceWriteOffset(size_t bytes) {

	this->_responseOffset += bytes;

	if (this->_responseOffset > this->_responseBuffer.size()) {
		this->_responseOffset = this->_responseBuffer.size();
	}
}

bool Client::isResponseFullySent() const {

	return !this->_responseBuffer.empty()
		&& this->_responseOffset >= this->_responseBuffer.size();
}