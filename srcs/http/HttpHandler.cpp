#include "HttpHandler.hpp"

HttpHandler::HttpHandler(const Request& request, const Location& location, const ServerConfig& server) : _server(server), _location(location), _request(request)
{
}

HttpHandler::HttpHandler(const HttpHandler& other) : 
_server(other._server), _location(other._location), _request(other._request)
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
