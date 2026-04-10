#pragma once

#include "HttpHandler.hpp"

class StaticHandler : public HttpHandler
{
  public:
    
    StaticHandler(const Request& request, const Location& location, const ServerConfig& server);
    ~StaticHandler();

  private:
    
    StaticHandler(const StaticHandler& other);
    StaticHandler& operator=(const StaticHandler& other);
};