#include "StaticHandler.hpp"

StaticHandler::StaticHandler(const Request& request, const Location& location, const ServerConfig& server) :
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
