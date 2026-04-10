#include "../inc/http/Request.hpp"

Request::Request()
{

}

Request::Request(int i)
{
    _port = i;
    _socketFd = i;
    _method = GET;
    _httpVersion = "1.0";
    _URI = "/exemple/fichier.txt";
    _body = "";
    _headers["host"] = "example.com";
}

Request::Request(const Request& other)
{
  *this = other;
}

Request::~Request()
{

}

Request& Request::operator=(const Request& other)
{
  if (this != &other)
  {
    _socketFd = other._socketFd;
    _method = other._method;
    _httpVersion = other._httpVersion;
    _URI = other._URI;
    _body = other._body;
    _headers = other._headers;
  }

  return *this;
}

// Functions
static char toLowerChar(char c)
{
  return std::tolower(static_cast<unsigned char>(c));
}

static const std::string& normalizeHeaderName(std::string& headerName)
{
  std::string normalizedHeaderName;

  std::transform(headerName.begin(), headerName.end(), normalizedHeaderName.begin(), toLowerChar);
  return headerName;
}

// Getters
int Request::getPort() const
{
  return _port;
}

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
  return _httpVersion;
}

const std::string& Request::getURI() const
{
  return _URI;
}

const std::string& Request::getBody() const
{
  return _body;
}

std::string Request::getHeader(const std::string& header) const
{
  std::map<std::string, std::string>::const_iterator it = _headers.find(header);

  if (it != _headers.end())
    return it->second;
  else
    return std::string();
}

// Setters
void Request::setPort(int port)
{
  _port = port;
}

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

void Request::setHeader(const std::string& header, std::string& value)
{
  _headers[header] = normalizeHeaderName(value);
}

// Methods
