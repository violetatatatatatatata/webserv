#include "ErrorHandler.hpp"
#include "Response.hpp"
#include <unistd.h>

ErrorHandler::ErrorHandler(const Request& request, const Location* location, const ServerConfig& server) :
HttpHandler(request, location, server)
{
}

ErrorHandler::ErrorHandler(const ErrorHandler& other) : HttpHandler(other)
{
}

ErrorHandler::~ErrorHandler() {}

// Methods

std::string buildError(int code, const ServerConfig& config);
std::string getFileContent(std::string path);

std::string ErrorHandler::getErrorBody(int error) const
{
    const std::string& path = buildError(error, _server);
    std::string content = "";

    if (HttpHandler::isFileInError(R_OK, path) == 0)
        content = getFileContent(path);

    return content;
}

void ErrorHandler::fillErrorResponse(int error, Response& response) const
{
    switch (error) 
    {
        case 403: response.setResponseData(403, "Forbidden", getErrorBody(403));
            break ;
        case 404: response.setResponseData(404, "Not Found", getErrorBody(404));
            break ;
        case 405: response.setResponseData(405, "Method Not Allowed", getErrorBody(405));
            break ;
        case 500: response.setResponseData(500, "Internal Server Error", getErrorBody(500));
            break ;
    }
}