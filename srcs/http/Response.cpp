#include <Response.hpp>

Response::Response() : _errorCode(0) { _cgiState.isCgi = true;}
Response::~Response() {}

Response::Response(const Response& other)
{
  *this = other;
}


Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
      _cgiState = other._cgiState;
      _errorCode = other._errorCode;
    }

    return *this;
}

// Methods
std::string Response::findMIME(const std::string& path) const
{
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos)
    {
        return "text/plain";
    }

    std::string ext = path.substr(pos);

    if (ext == ".html" || ext == ".htm") return "text/html";
    if (ext == ".css")  return "text/css";
    if (ext == ".js")   return "application/javascript";
    if (ext == ".png")  return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".gif")  return "image/gif";
    if (ext == ".txt")  return "text/plain";
    if (ext == ".json") return "application/json";
    
    return "text/plain";
}

void Response::fillHeaders()
{
    std::ostringstream oss;
    oss << _body.size();
    setHeader("Content-Length", oss.str());
    setHeader("Connection", "close");
}

std::string Response::buildResponse()
{
  fillHeaders();

  std::ostringstream oss;
  oss << _errorCode;
  std::string responseString = _version + " " + oss.str() + " " + _reasonPhrase + "\r\n";

  std::map<std::string, std::string>::const_iterator it;
  for (it = _headers.begin(); it != _headers.end(); it++)
  {
    responseString.append(it->first + ": " + it->second + "\r\n");
  }

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
  _errorCode = error;
  _reasonPhrase = reasonPhrase;
  _body = body;
}

void Response::setHeader(const std::string& header, const std::string& value)
{
  _headers[header] = value;
}

void Response::setCGIState(pid_t cgiPid, int pipeFd, int socketFd)
{
  _cgiState.cgiPid = cgiPid;
  _cgiState.pipeFd = pipeFd;
  _cgiState.socketFd = socketFd;
}

// Getters
const CGIState& Response::getCGIState() const
{
  return _cgiState;
}