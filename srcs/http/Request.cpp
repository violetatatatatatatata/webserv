#include "Request.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

Request::Request(const std::string& raw)
{
  parse(raw);
}

Request::~Request() {}

Request::Request(int i)
{
    _port = i;
    _socketFd = i;
    _method = "DELETE";
    _httpVersion = "HTTP/1.0";
    _URI = "/example/file.txt";
    _body = "";
    _headers["host"] = "localhost";
}

Request::Request(const Request& other)
{
  *this = other;
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
    std::transform(headerName.begin(),
                   headerName.end(),
                   headerName.begin(),
                   toLowerChar);

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

const std::string& Request::getMethod() const
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

void Request::setMethod(const std::string& method)
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
void Request::parse(const std::string& raw)
{
    std::istringstream stream(raw);
    std::string line;

    // 1. Request line
    if (!std::getline(stream, line))
        throw std::runtime_error("Empty request");

    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    std::istringstream reqLine(line);
    std::string method, uri, version;

    reqLine >> method >> uri >> version;

    if (method.empty() || uri.empty() || version.empty())
        throw std::runtime_error("Malformed request line");

    setMethod(method);
    setURI(uri);
    setVersion(version);

    // 2. Headers
    while (std::getline(stream, line))
    {
        if (line == "\r" || line.empty())
            break;

        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        size_t pos = line.find(':');
        if (pos == std::string::npos)
            continue; // Invalid header

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // trim left spaces
        while (!value.empty() && value[0] == ' ')
            value.erase(0, 1);

        setHeader(key, value);
    }

    // 3. Body
    std::string body;
    while (std::getline(stream, line))
    {
        body += line;
        body += "\n";
    }

    if (!body.empty() && body[body.size() - 1] == '\n')
        body.erase(body.size() - 1);

    setBody(body);
    // setPort(8080); LA FUNCION DEBE RECIBIR EL SOCKET
}