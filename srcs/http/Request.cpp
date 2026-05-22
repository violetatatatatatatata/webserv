#include <Request.hpp>

Request::Request(){}
Request::~Request() {}

// Functions
/*static char toLowerChar(char c)
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
}*/

// Getters
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

void Request::setHeader(const std::string& header, const std::string& value)
{
  _headers[header] = value;
}

// Methods
int Request::parse(const std::string& raw)
{
    std::istringstream stream(raw);
    std::string line;

    // 1. Request line
    if (!std::getline(stream, line))
      return -1; //throw std::runtime_error("Empty request");

    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    std::istringstream reqLine(line);
    std::string method, uri, version;

    reqLine >> method >> uri >> version;

    if (method.empty() || uri.empty() || version.empty())
      return -1; //throw std::runtime_error("Malformed request line");

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

    // Check if Content-Length header is present with POST
    if (getMethod() == "POST" && getHeader("Content-Length").empty())
      return -1;

    // 3. Body
    std::string body;
    while (std::getline(stream, line))
    {
        body += line;
        body += "\n";
    }

    if (!body.empty() && body[body.size() - 1] == '\n')
        body.erase(body.size() - 1);

    return (setBody(body), 0);
}

void Request::print()
    {
      		std::cout << _method << std::endl;

    for (std::map<std::string, std::string>::iterator it = _headers.begin();
         it != _headers.end();
         ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
            		std::cout << _body << std::endl;

    }
