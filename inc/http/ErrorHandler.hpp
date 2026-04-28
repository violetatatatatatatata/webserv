#pragma once

#include "HttpHandler.hpp"

class ServerConfig;

class ErrorHandler : public HttpHandler
{
  public:

    ErrorHandler(const Request& request, const Location* location, const ServerConfig& server);
    ErrorHandler(const ErrorHandler& other);
    ~ErrorHandler();
    
    void fillErrorResponse(int error, Response& response) const;

  private:
    std::string getErrorBody(int error) const;
};