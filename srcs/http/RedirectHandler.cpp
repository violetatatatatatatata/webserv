#include "RedirectHandler.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Request.cpp"

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
    response.setVersion(_request.getVersion());    
    response.setResponseData(301, "Moved Permanently", "");
    
    // Corregir -> actualmente no es posible tener request sin un bloque location
    //if (_location == NULL)
    //    response.setHeader("Location", "");
    //else
        response.setHeader("Location", _location->getRedirect());
}