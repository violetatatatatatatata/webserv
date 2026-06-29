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
	_responseBuffer(""), _responseOffset(0),
	_parseState(READING_HEADERS), _headerEndPos(0),
	_expectedBodySize(0), _bodyTooLarge(false),
	_maxBodySize(0), _isChunked(false), _chunkedSearchPos(0), _lastActivity(time(NULL)) {}

Client::Client(int fd, int serverFd)
	: _fd(fd), _serverFd(serverFd),
	_requestBuffer(""),
	_responseBuffer(""), _responseOffset(0),
	_parseState(READING_HEADERS), _headerEndPos(0),
	_expectedBodySize(0), _bodyTooLarge(false),
	_maxBodySize(0), _isChunked(false), _chunkedSearchPos(0), _lastActivity(time(NULL)) {}

Client::Client(const Client& other) {

	*this = other;
}

Client& Client::operator=(const Client& other) {

	if (this != &other) {
		this->_fd = other._fd;
		this->_serverFd = other._serverFd;
		this->_requestBuffer = other._requestBuffer;
		this->_responseBuffer = other._responseBuffer;
		this->_responseOffset = other._responseOffset;
		this->_parseState = other._parseState;
		this->_headerEndPos = other._headerEndPos;
		this->_expectedBodySize = other._expectedBodySize;
		this->_bodyTooLarge = other._bodyTooLarge;
		this->_lastActivity = other._lastActivity;
		this->_maxBodySize = other._maxBodySize;
		this->_isChunked = other._isChunked;
		this->_chunkedSearchPos = other._chunkedSearchPos;
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

void Client::extractContentLength() {

	size_t clPos = _requestBuffer.find("Content-Length:");
	if (clPos == std::string::npos || clPos >= _headerEndPos)
		return;

	size_t valuePos = clPos + 15;
	size_t endLinePos = _requestBuffer.find("\r\n", valuePos);
	if (endLinePos == std::string::npos || endLinePos >= _headerEndPos)
		return;

	std::string clStr = _requestBuffer.substr(valuePos, endLinePos - valuePos);
	_expectedBodySize = std::strtoul(clStr.c_str(), NULL, 10);
}

void Client::processHeaders() {

	size_t pos = _requestBuffer.find("\r\n\r\n");
	if (pos == std::string::npos)
		return;

	_headerEndPos = pos + 4;
	extractContentLength();

	size_t tePos = _requestBuffer.find("Transfer-Encoding: chunked");
	if (tePos != std::string::npos && tePos < _headerEndPos) {
		_isChunked  = true;
		_parseState = READING_CHUNKED;
		return;
	}

	size_t maxBodySize = _maxBodySize;

	std::cout << "Exected: " << _expectedBodySize << " Max: " << maxBodySize << std::endl;
	if (_expectedBodySize > maxBodySize) {
		_bodyTooLarge = true;
		_parseState   = READY;
		return;
	}

	if (_expectedBodySize > 0)
		_parseState = READING_BODY;
	else
		_parseState = READY;
}

void Client::processBody() {

	size_t currentBodySize = _requestBuffer.size() - _headerEndPos;

	if (currentBodySize >= _expectedBodySize)
		_parseState = READY;
}

void Client::processChunkedBody() {

    size_t searchFrom = (_chunkedSearchPos > 4) ? (_chunkedSearchPos - 4) : 0;
    if (_requestBuffer.find("0\r\n\r\n", searchFrom) != std::string::npos)
        _parseState = READY;
    _chunkedSearchPos = _requestBuffer.size();
}

bool Client::isRequestComplete() {

    if (_parseState == READING_HEADERS)
        processHeaders();

    if (_parseState == READING_BODY)
        processBody();

    if (_parseState == READING_CHUNKED)
        processChunkedBody();

    return _parseState == READY;
}

void Client::setResponse(const std::string& response) {

	this->_responseBuffer = response;
	this->_responseOffset = 0;
}

void Client::setMaxBodySize(size_t size) { 
	
	_maxBodySize = size; 
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

bool Client::isBodyTooLarge() const {
	return _bodyTooLarge;
}

size_t Client::getMaxBodySize() const {
	return _maxBodySize;
}

time_t Client::getLastActivity() const {
	return _lastActivity;
}

bool Client::isChunked() const {
	return _isChunked;
}

void Client::updateActivity() {
	_lastActivity = time(NULL);
}

