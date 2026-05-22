#pragma once

#include "HttpHandler.hpp"

class RedirectHandler : public HttpHandler
{
  public:

    RedirectHandler(const Request& request, const LocationParser* location, const ServerParser& server);
    ~RedirectHandler();

    void handleRequest(Response& response);
    
  private:
  
    RedirectHandler(const RedirectHandler& other);
};