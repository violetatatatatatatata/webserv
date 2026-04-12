#include "CGIHandler.hpp"

CGIHandler::CGIHandler(const Request& request, const Location* location, const ServerConfig& server) :
HttpHandler(request, location, server)
{
}

CGIHandler::~CGIHandler()
{
}

CGIHandler& CGIHandler::operator=(const CGIHandler& other)
{
    if (this != &other) {

    }
    return *this;
}

// Methods
void CGIHandler::handleRequest(Response& response)
{
    (void)response;
}