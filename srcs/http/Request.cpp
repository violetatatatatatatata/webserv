#include "../inc/http/Request.hpp"

Request::Request()
{

}

Request::Request(const Request& other)
{

}

Request::~Request()
{

}

Request& Request::operator=(const Request& other)
{

}

// Getters
int Request::getSocketFd() const
{
  return _socketFd;
}

e_method Request::getMethod() const
{
  return _method;
}

const std::string& Request::getVersion() const
{
  return httpVersion;
}

const std::string& Request::getURI() const
{
  return _URI;
}

const std::string& Request::getBody() const
{
  return _body;
}

const std::string& Request::getHeader(const std::string& header) const
{
  std::map<std::string, std::string>::iterator it = _headers.find(header);

  //if (*it != _headers.end())
    return *it;

  // peut etre mettre .at() ici pour utiliser l'exception
}

// Setters
void Request::setSocketFd(int socketFd)
{
  _socketFd = socketFd;
}

void Request::setMethod(e_method method)
{
  _method = method;
}

void Request::setVersion(std::string& httpVersion)
{
  _httpVersion = httpVersion;
}

void Request::setURI(std::string& URI)
{
  _URI = URI;
}

void Request::setBody(std::string& body)
{
  _body = body;
}

void Request::setHeader(const std::string& header, const std::string& value)
{
  _headers[header] = value;
}

// Methods

