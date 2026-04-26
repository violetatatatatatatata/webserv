#include "Response.hpp"
#include <sstream>

Response::Response() : _error_code(0) {}
Response::~Response() {}

Response::Response(const Response& other)
{
  *this = other;
}


Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
      _socket_Fd = other._socket_Fd;
      _error_code = other._error_code;
    }
    
    return *this;
}

// Methods
void Response::fillHeaders()
{
    std::ostringstream oss;
    oss << _body.size();
    setHeader("Content-Length", oss.str());
    setHeader("Connection", "close");
    setHeader("Content-Type", "text/html");
}

std::string Response::buildResponse()
{
  fillHeaders();
  
  std::ostringstream oss;
  oss << _error_code;
  std::string responseString = _version + " " + oss.str() + " " + _reasonPhrase + "\r\n";

  std::map<std::string, std::string>::const_iterator it;
  for (it = _headers.begin(); it != _headers.end(); it++)
    responseString.append(it->first + ": " + it->second + "\r\n");

  responseString.append("\r\n" + _body);
  return responseString;
}

// Setters
void Response::setVersion(const std::string& version)
{
  _version = version;
}

void Response::setResponseData(int error, const std::string& reasonPhrase, const std::string& body)
{
  _error_code = error;
  _reasonPhrase = reasonPhrase;
  _body = body;
}

void Response::setHeader(const std::string& header, const std::string& value)
{
  _headers[header] = value;
}

void Response::printResponse()
{
  std::cout << "\nResponse:\n " << std::endl;
  std::cout << _version << " " << _error_code << " " << _reasonPhrase << std::endl;
  std::cout << "Content-Length: " << _headers["Content-Length"] << std::endl;
  std::cout << "Connection: " << _headers["Connection"] << std::endl;
  std::cout << "Content-Type: " << _headers["Content-Type"] << std::endl;
  std::cout << "\n" << _body << std::endl;
}