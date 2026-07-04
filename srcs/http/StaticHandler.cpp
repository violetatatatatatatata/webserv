#include "StaticHandler.hpp"
#include "ErrorHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "LocationParser.hpp"
#include <fcntl.h>
#include <cstdio>

StaticHandler::StaticHandler(const Request& request, const LocationParser* location, const ServerParser& server, const std::string& absolute_path) :
HttpHandler(request, location, server), _absolute_path(absolute_path)
{
}

StaticHandler::~StaticHandler() {}

bool StaticHandler::isMethodAuthorized() const
{
    const std::string& method = _request.getMethod();

    if (_location == NULL || _location->getMethods().empty())
        return method == "GET";

    std::vector<std::string>::const_iterator it;
    for (it = _location->getMethods().begin(); it != _location->getMethods().end(); ++it)
    {
        if (*it == method)
            return true;
    }
    return false;
}

void StaticHandler::handleRequest(Response& response)
{
    response.setVersion(_request.getVersion());

    if (!isMethodAuthorized())
    {
        ErrorHandler errorResponse(405, _request, _server, response);
        return ;
    }
    
    const std::string& method = _request.getMethod();
    if (method == "GET")
        handleGET(response);
    else if (method == "DELETE")
        handleDELETE(response);
    else if (method == "POST")
        handlePOST(response);
    else
        ErrorHandler(405, _request, _server, response);
}

std::string getFileContent(std::string path);

void StaticHandler::handleGET(Response& response) const
{
    int res = isFileInError(R_OK, _absolute_path);
    if (res != 0 )
    {
        ErrorHandler errorResponse(res, _request, _server, response);
        return ;
    }

    std::string content = getFileContent(_absolute_path);

    response.setResponseData(200, "OK", content);
    response.setHeader("Content-Type", response.findMIME(_absolute_path));
}

void StaticHandler::handleDELETE(Response& response) const
{
    int ret = std::remove(_absolute_path.c_str());

    if (ret == 0)
        response.setResponseData(204, "No Content", "");
    else
    {
        switch (errno)
        {
            case ENOENT:
            {
                ErrorHandler errorResponse(404, _request, _server, response);
                break;
            }
            case EACCES:
            case EPERM:
            {
                ErrorHandler errorResponse(403, _request, _server, response);
                break;
            }

            default:
            {
                ErrorHandler errorResponse(500, _request, _server, response);
                break;
            }
        }
    }
}

void StaticHandler::handlePOST(Response& response) const
{
    response.setResponseData(200, "OK", "");
}
