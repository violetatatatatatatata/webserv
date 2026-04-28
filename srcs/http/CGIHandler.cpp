#include "CGIHandler.hpp"
#include <iostream>

CGIHandler::CGIHandler(const Request& request, const Location* location, const ServerConfig& server) :
HttpHandler(request, location, server)
{
}

CGIHandler::~CGIHandler()
{
}

// Methods
void CGIHandler::handleRequest(Response& response)
{
    std::cout << "CGI request !" << std::endl;
    (void)response;
}