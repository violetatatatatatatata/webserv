#include "HttpHandler.hpp"

HttpHandler::HttpHandler(const Request& request, const Location* location, const ServerConfig& server) : _request(request), _location(location), _server(server)
{
}

HttpHandler::HttpHandler(const HttpHandler& other) : 
_request(other._request), _location(other._location), _server(other._server)
{
}

HttpHandler::~HttpHandler()
{
}

HttpHandler& HttpHandler::operator=(const HttpHandler& other)
{
    if (this != &other) {

    }
    return *this;
}