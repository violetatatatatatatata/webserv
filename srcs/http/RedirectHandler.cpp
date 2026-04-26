#include "RedirectHandler.hpp"
#include "Response.hpp"

RedirectHandler::RedirectHandler(const Request& request, const Location* location, const ServerConfig& server) : HttpHandler(request, location, server)
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

// Methods
void RedirectHandler::handleRequest(Response& response)
{
    response.setResponseData(301, "file", "file"); // to fix
}