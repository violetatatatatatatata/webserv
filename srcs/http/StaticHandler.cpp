#include "StaticHandler.hpp"
#include "ErrorHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include <fcntl.h>
#include <cstdio>

StaticHandler::StaticHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& absolute_path) :
HttpHandler(request, location, server), _absolute_path(absolute_path)
{
}

StaticHandler::~StaticHandler() {}

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

void StaticHandler::handleRequest(Response& response)
{
    response.setVersion(_request.getVersion());

    if (!isMethodAuthorized())
    {
        ErrorHandler errorResponse(405, _request, _server, response);
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

std::string getFileContent(std::string path);

void StaticHandler::handleGET(Response& response) const
{
    int res = isFileInError(R_OK, _absolute_path);
    if (res != 0)
    {
        ErrorHandler errorResponse(res, _request, _server, response);
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
        ErrorHandler errorResponse(res, _request, _server, response);
        return ;
    }

    std::remove(_absolute_path.c_str());
    response.setResponseData(204, "No Content", "");
}