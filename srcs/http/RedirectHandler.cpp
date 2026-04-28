#include "RedirectHandler.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include <sstream>

RedirectHandler::RedirectHandler(const Request& request, const Location* location, const ServerConfig& server) : HttpHandler(request, location, server)
{
}

RedirectHandler::~RedirectHandler()
{
}

// Methods
void RedirectHandler::handleRequest(Response& response)
{
    std::cout << "The request will be resolved as a Redirection." << std::endl;
    response.setVersion(_request.getVersion());    

    int code;
    std::string path;

    std::string input = _location->getRedirect();
    std::istringstream iss(input);
    iss >> code >> path;
    
    response.setResponseData(code, "Moved Permanently", ""); // Change message
    
    // Corregir -> actualmente no es posible tener request sin un bloque location
    //if (_location == NULL)
    //    response.setHeader("Location", "");
    //else
    //std::cout << "path" << path << std::endl;
    response.setHeader("Location", path);
}