#pragma once

#include "HttpHandler.hpp"

class RedirectHandler : public HttpHandler
{
  public:

    RedirectHandler(const Request& request, const Location* location, const ServerConfig& server);
    ~RedirectHandler();

    void resolveRequest();
    
  private:
  
    RedirectHandler& operator=(const RedirectHandler& other);
    RedirectHandler(const RedirectHandler& other);
};