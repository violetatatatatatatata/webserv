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

static std::string normalizeSlashes(std::string &path)
{
    std::string result;
    bool prevSlash = false;

    for (size_t i = 0; i < path.size(); i++)
    {
        if (path[i] == '/')
        {
            if (!prevSlash)
                result += '/';
            prevSlash = true;
        }
        else
        {
            result += path[i];
            prevSlash = false;
        }
    }
    return result;
}

// Getters
int Request::getclientFd() const
{
  return _clientFd;
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

bool Request::getIsChunked() const
{
  return _isChunked;
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
  return _headers;
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
void Request::setclientFd(int clientFd)
{
  _clientFd = clientFd;
}

void Request::setMethod(const std::string& method)
{
  _method = method;
}

void Request::setVersion(std::string& httpVersion)
{
  _httpVersion = httpVersion;
}

void Request::setURI(const std::string& URI)
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
bool Request::isChunked() const
{
    std::map<std::string, std::string>::const_iterator it =
        _headers.find("Transfer-Encoding");

    if (it == _headers.end())
        return false;

    const std::string &value = it->second;

    return (value.find("chunked") != std::string::npos);
}

int Request::parse(const std::string& raw)
{
    std::istringstream stream(raw);
    std::string line;

    if (!std::getline(stream, line))
      return -1;

    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    std::istringstream reqLine(line);
    std::string method, uri, version;

    reqLine >> method >> uri >> version;

    if (method.empty() || uri.empty() || version.empty())
      return -1;

    setMethod(method);
    setURI(normalizeSlashes(uri));
    setVersion(version);

    while (std::getline(stream, line))
    {
        if (line == "\r" || line.empty())
            break;

        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        size_t pos = line.find(':');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        while (!value.empty() && value[0] == ' ')
            value.erase(0, 1);

        setHeader(key, value);
    }

    if (isChunked())
      _isChunked = true;
    else
      _isChunked = false;

    if (getMethod() == "POST" && _headers.find("Content-Length") == _headers.end() && !_isChunked)
      return -1;

    // Find body start in the raw buffer (after \r\n\r\n)
    size_t hdrEnd = raw.find("\r\n\r\n");
    if (hdrEnd == std::string::npos)
    {
        std::string empty;
        return (setBody(empty), 0);
    }
    size_t bodyStart = hdrEnd + 4;

    std::string body;

    if (_isChunked)
    {
        // Decode chunked transfer encoding
        size_t pos = bodyStart;
        while (pos < raw.size())
        {
            size_t lineEnd = raw.find("\r\n", pos);
            if (lineEnd == std::string::npos)
                break;
            std::string sizeLine = raw.substr(pos, lineEnd - pos);
            // Strip chunk extensions (anything after ';')
            size_t semi = sizeLine.find(';');
            if (semi != std::string::npos)
                sizeLine = sizeLine.substr(0, semi);
            char *endptr;
            size_t chunkSize = std::strtoul(sizeLine.c_str(), &endptr, 16);
            if (endptr == sizeLine.c_str())
                break;
            pos = lineEnd + 2;
            if (chunkSize == 0)
                break;
            if (pos + chunkSize > raw.size())
                break;
            body.append(raw, pos, chunkSize);
            pos += chunkSize + 2; // skip chunk data + \r\n
        }
    }
    else
    {
        // Use Content-Length to extract the exact body
        std::string clStr = getHeader("Content-Length");
        size_t contentLength = std::strtoul(clStr.c_str(), NULL, 10);
        if (bodyStart < raw.size())
            body = raw.substr(bodyStart, contentLength);
    }

    return (setBody(body), 0);
}

/*void Request::print()
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
*/