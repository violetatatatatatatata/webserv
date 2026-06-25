#include "RedirectHandler.hpp"
#include "Response.hpp"
#include "LocationParser.hpp"
#include "Request.hpp"
#include <sstream>

RedirectHandler::RedirectHandler(const Request& request, const LocationParser* location, const ServerParser& server) : HttpHandler(request, location, server)
{
}

RedirectHandler::~RedirectHandler()
{
}

// Methods
void RedirectHandler::handleRequest(Response& response)
{
    response.setVersion(_request.getVersion());

    int code;
    std::string path;

    std::string input = _location->getRedirect();
    std::istringstream iss(input);
    iss >> code >> path;

    std::cout << input << std::endl;
    response.setResponseData(code, "Moved Permanently", "");
    response.setHeader("Location", path);
}
