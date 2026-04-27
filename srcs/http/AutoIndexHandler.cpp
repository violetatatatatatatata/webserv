#include "AutoIndexHandler.hpp"

AutoIndexHandler::AutoIndexHandler(
    const Request& request,
    const Location* location,
    const ServerConfig& server,
    const std::string& directory) : HttpHandler(request, location, server), _directory(directory)
{
}

AutoIndexHandler::~AutoIndexHandler() {}

AutoIndexHandler& AutoIndexHandler::operator=(const AutoIndexHandler& other)
{
    if (this != &other) {

    }
    return *this;
}

// Methods
void AutoIndexHandler::handleRequest(Response& response)
{
    /*std::string files = 
    "<html>
    <head>
    </head>
    <body>
    </body>
    </html>";
    */
    (void)response;
}