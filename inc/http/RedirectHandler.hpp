#pragma once

#include "HttpHandler.hpp"

class RedirectHandler : public HttpHandler
{
  public:

    RedirectHandler(const Request& request, const Location* location, const ServerConfig& server);
    ~RedirectHandler();

    void handleRequest(Response& response);
    
  private:
  
    RedirectHandler(const RedirectHandler& other);
};