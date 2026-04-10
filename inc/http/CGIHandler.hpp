#pragma once

#include "HttpHandler.hpp"

class ServerConfig;

class CGIHandler : public HttpHandler
{
  public:
    CGIHandler(const Request& request, const Location& location, const ServerConfig& server);
    ~CGIHandler();
    
  private:
  
    CGIHandler(const CGIHandler& other);
    CGIHandler& operator=(const CGIHandler& other);
    
};