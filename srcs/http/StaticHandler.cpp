#include "StaticHandler.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "ServerConfig.hpp"

StaticHandler::StaticHandler(const Request& request, const Location* location, const ServerConfig& server) :
HttpHandler(request, location, server)
{
}

StaticHandler::~StaticHandler()
{
}

StaticHandler& StaticHandler::operator=(const StaticHandler& other)
{
    if (this != &other) {

    }
    return *this;
}

// Methods
void StaticHandler::resolveRequest()
{
    resolveAbsolutePath();
    
    if (!isMethodAuthorized())
    {
        //error_code = 405;
        return ;
    }
    
    std::string methods[] =
    {
        "GET",
        "POST",
        "DELETE"
    };

    void (StaticHandler::*methodFunctions[3])(void) const =
    {
        &StaticHandler::resolveGET,
        &StaticHandler::resolvePOST,
        &StaticHandler::resolveDELETE
    };

    for (int i = 0; i < 3; i++)
    {
        if (_request.getMethod() == methods[i])
        {
            (this->*methodFunctions[i])();
        }
    }

}

void StaticHandler::resolveAbsolutePath()
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
}

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

bool StaticHandler::isFileExistant() const
{
    return false;
}

bool StaticHandler::isFileReadable() const
{
    return false;
}

void StaticHandler::resolveGET() const
{
    print_msg(GREEN "The request will be resolved as a static GET." RESET, DEBUG);

    if (!isFileExistant())
    {
        //error_code = 404;
    }

    if (!isFileReadable())
    {
        //error_code = 403;
    }
}

void StaticHandler::resolvePOST() const
{
     print_msg(GREEN "The request will be resolved as a static POST." RESET, DEBUG);
   
}

void StaticHandler::resolveDELETE() const
{
     print_msg(GREEN "The request will be resolved as a static DELETE." RESET, DEBUG);
}

