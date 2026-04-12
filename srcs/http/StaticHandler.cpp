#include "StaticHandler.hpp"
#include "ServerConfig.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

StaticHandler::StaticHandler(const Request& request, const Location* location, const ServerConfig& server) :
HttpHandler(request, location, server)
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
        {
            return true;
        }
    }
    else
    {
        std::vector<std::string>::const_iterator it;
        for(it = _location->getMethods().begin(); it < _location->getMethods().end(); it++)
        {
            if (*it == _request.getMethod())
            {
                return true;
            }
        }
    }
    return false;
}

int StaticHandler::checkStat(struct stat& st) const
{
    if (stat(_absolute_path.c_str(), &st) == -1)
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

// Return empty string if no indexes
static const std::string& getCorrectIndex(const std::string& location_index, const std::string& server_index)
{
    if (location_index.empty())
    {
        return server_index;
    }
    else
    {
        return location_index;
    }
}

// Resolve the directory path
int StaticHandler::resolveDirectory()
{
    // No vector de index en Location y ServerConfig?
    //const std::vector<std::string>& index = getCorrectIndex(_location->getIndex(), _server.getIndex());
    const std::string& index = getCorrectIndex(_location->getIndex(), _server.getIndex());

    if (index.empty())
    {
        return 404;
    }

    _absolute_path += index;

    return 0;
}

int StaticHandler::resolveAbsolutePath()
{
    // If no location bloc or no root in location bloc -> take the root from server
    if (_location == NULL || _location->getRoot().empty())
    {
        _absolute_path = _server.getRoot() + _request.getURI();
    }
    else
    {
        _absolute_path = _location->getRoot() + _request.getURI();
    }

    struct stat st;

    int res = checkStat(st);
    if (res != 0)
    {
        return res;
    }

    if (S_ISDIR(st.st_mode) && resolveDirectory() == 404)
    {
        return 404;
    }

    return 0;
}

int StaticHandler::isFileInError() const
{
    struct stat st;

    int res = checkStat(st);
    if (res != 0)
    {
        return res;
    }

    if (S_ISDIR(st.st_mode))
    {
            return 404;
    }

    if (access(_absolute_path.c_str(), R_OK) == -1)
    {
        return 403;
    }

    return 0;
}

void StaticHandler::handleRequest(Response& response)
{
    if (!isMethodAuthorized())
    {
        response.setError(405);
        print_msg(RED "Method authorized." RESET, DEBUG);
        return ;
    }
    print_msg(GREEN "Method authorized." RESET, DEBUG);

    int res = resolveAbsolutePath();
    if (res != 0)
    {
        print_msg(RED "Path error." RESET, DEBUG);
        response.setError(res);
        return ;
    }
    print_msg(std::string(GREEN) + "Absolute path resolved: " + _absolute_path + RESET, DEBUG);

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
        {
            (this->*methodFunctions[i])(response);
        }
    }

}

void StaticHandler::handleGET(Response& response) const
{
    print_msg(GREEN "The request will be resolved as a static GET." RESET, DEBUG);

    int res = isFileInError();
    if (res != 0)
    {
        response.setError(res);
        return ;
    }
}

void StaticHandler::handlePOST(Response& response) const
{
    print_msg(GREEN "The request will be resolved as a static POST." RESET, DEBUG);
    (void)response;
}

void StaticHandler::handleDELETE(Response& response) const
{
    print_msg(GREEN "The request will be resolved as a static DELETE." RESET, DEBUG);
    (void)response;
}

