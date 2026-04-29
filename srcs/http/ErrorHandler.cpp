#include "ErrorHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include <unistd.h>

ErrorHandler::ErrorHandler(int error, const Request& request, const ServerConfig& server, Response& response) :
HttpHandler(request, NULL, server), _error(error)
{
    handleRequest(response);
}

ErrorHandler::ErrorHandler(const ErrorHandler& other) : HttpHandler(other)
{
}

ErrorHandler::~ErrorHandler() {}

// Methods

std::string buildError(int code, const ServerConfig& config);
std::string getFileContent(std::string path);

std::string ErrorHandler::getErrorBody() const
{
    const std::string& path = buildError(_error, _server);
    std::string content = "";

    if (HttpHandler::isFileInError(R_OK, path) == 0)
        content = getFileContent(path);

    return content;
}

void ErrorHandler::fillErrorResponse(Response& response) const
{
    response.setVersion(_request.getVersion());

    switch (_error) 
    {
        case 403: response.setResponseData(403, "Forbidden", getErrorBody());
            break ;
        case 404: response.setResponseData(404, "Not Found", getErrorBody());
            break ;
        case 405: response.setResponseData(405, "Method Not Allowed", getErrorBody());
            break ;
        case 500: response.setResponseData(500, "Internal Server Error", getErrorBody());
            break ;
    }
}

// Methods
void ErrorHandler::handleRequest(Response& response)
{
    std::cout << "ERROR !" << std::endl;
    fillErrorResponse(response);
}