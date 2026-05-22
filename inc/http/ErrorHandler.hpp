#pragma once

#include "HttpHandler.hpp"

class ServerParser;

class ErrorHandler : public HttpHandler
{
  public:

    ErrorHandler(int error, const Request& request, const ServerParser& server, Response& response);
    ErrorHandler(const ErrorHandler& other);
    ~ErrorHandler();
    
    void fillErrorResponse(Response& response) const;

  private:

    void handleRequest(Response& response);
    std::string getErrorBody() const;
    
    int _error;
};