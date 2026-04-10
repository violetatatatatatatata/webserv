#include "AutoIndexHandler.hpp"

AutoIndexHandler::AutoIndexHandler(const Request& request, const Location& location, const ServerConfig& server) : HttpHandler(request, location, server)
{
}

AutoIndexHandler::~AutoIndexHandler()
{
}

AutoIndexHandler& AutoIndexHandler::operator=(const AutoIndexHandler& other)
{
    if (this != &other) {

    }
    return *this;
}
