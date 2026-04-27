#include "StaticHandler.hpp"
#include "Error.hpp"

StaticHandler::StaticHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& absolute_path) :
HttpHandler(request, location, server), _absolute_path(absolute_path)
{
}

StaticHandler::~StaticHandler() {}

StaticHandler& StaticHandler::operator=(const StaticHandler& other)
{
    if (this != &other) {

    }
    return *this;
}

// Methods
bool StaticHandler::isMethodAuthorized() const
{
    // If no methods defined in config -> only GET is authorized
    //--> El methods fuera de location ne esta implementado
    if (_location == NULL || _location->getMethods().empty())
    {
        if (_request.getMethod() == "GET")
            return true;
    }
    else
    {
        std::vector<std::string>::const_iterator it;
        for(it = _location->getMethods().begin(); it < _location->getMethods().end(); it++)
        {
            if (*it == _request.getMethod())
                return true;
        }
    }
    return false;
}

static int checkStat(struct stat& st, const std::string& file)
{
    if (stat(file.c_str(), &st) == -1)
    {
        switch(errno)
        {
            case ENOENT:
            case ENOTDIR:
                return 404;
            case EACCES:
                return 403;
            default:
                return 500;
        }
    }
    return 0;
}

static int isFileInError(int mode, const std::string file)
{
    struct stat st;

    int res = checkStat(st, file);
    if (res != 0)
        return res;

    if (S_ISDIR(st.st_mode))
            return 404;

    if (access(file.c_str(), mode) == -1)
        return 403;

    return 0;
}

static std::string getFileContent(std::string path)
{
    int fd = open(path.c_str(), O_RDONLY);
    char buffer[4096];
    std::string content;
    ssize_t bytes;
    std::stringstream ss;

    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
    {
        content.append(buffer, bytes);
    }
    
    return content;
}

std::string buildError(int code, const ServerConfig& config);
std::string StaticHandler::getErrorBody(int error) const
{
    const std::string& path = buildError(error, _server);
    std::string content = "";

    if (isFileInError(R_OK, path) == 0)
        content = getFileContent(path);

    return content;
}

void StaticHandler::fillErrorResponse(int error, Response& response) const
{
    switch (error) 
    {
        case 301: response.setResponseData(301, "Moved Permanently", getErrorBody(301));
            break ;
        case 403: response.setResponseData(403, "Forbidden", getErrorBody(403));
            break ;
        case 404: response.setResponseData(404, "Not Found", getErrorBody(404));
            break ;
        case 405: response.setResponseData(405, "Method Not Allowed", getErrorBody(405));
            break ;
        case 500: response.setResponseData(500, "Internal Server Error", getErrorBody(500));
            break ;
    }
}

void StaticHandler::handleRequest(Response& response)
{
    response.setVersion(_request.getVersion());

    if (!isMethodAuthorized())
    {
        fillErrorResponse(405, response);
        return ;
    }

    std::string methods[] =
    {
        "GET",
        "POST",
        "DELETE"
    };

    void (StaticHandler::*methodFunctions[3])(Response&) const =
    {
        &StaticHandler::handleGET,
        &StaticHandler::handlePOST,
        &StaticHandler::handleDELETE
    };

    for (int i = 0; i < 3; i++)
    {
        if (_request.getMethod() == methods[i])
            (this->*methodFunctions[i])(response);
    }

}

void StaticHandler::handleGET(Response& response) const
{
    int res = isFileInError(R_OK, _absolute_path);
    if (res != 0)
    {
        fillErrorResponse(res, response);
        return ;
    }

    std::string content = getFileContent(_absolute_path);
    response.setResponseData(200, "OK", content);
    response.setHeader("Content-Type", response.findMIME(_absolute_path)); 
}

void StaticHandler::handlePOST(Response& response) const
{
    handleGET(response);
}

void StaticHandler::handleDELETE(Response& response) const
{
    int res = isFileInError(F_OK, _absolute_path); 
    if (res != 0)
    {
        fillErrorResponse(res, response);
        return ;
    }

    std::remove(_absolute_path.c_str());
    response.setResponseData(204, "No Content", "");
}