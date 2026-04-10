#include "RedirectHandler.hpp"

RedirectHandler::RedirectHandler(const Request& request, const Location& location, const ServerConfig& server) : HttpHandler(request, location, server)
{
}

RedirectHandler::~RedirectHandler()
{
}

RedirectHandler& RedirectHandler::operator=(const RedirectHandler& other)
{
    if (this != &other) {
        
    }
    return *this;
}
