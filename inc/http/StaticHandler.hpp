#pragma once

#include "HttpHandler.hpp"
#include <string>

class Response;

class StaticHandler : public HttpHandler
{
  public:
    
    StaticHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& absolute_path);
    ~StaticHandler();

    void handleRequest(Response& response);

  private:
    
    StaticHandler(const StaticHandler& other);
    StaticHandler& operator=(const StaticHandler& other);

    bool  isMethodAuthorized() const;
    int   resolveDirectory();
    int   resolveAbsolutePath();
    void  handleGET(Response& response) const;
    void  handlePOST(Response& response) const;
    void  handleDELETE(Response& response) const;
    void  fillErrorResponse(int error, Response& response) const;
    std::string getErrorBody(int error) const;

    std::string _absolute_path;
};