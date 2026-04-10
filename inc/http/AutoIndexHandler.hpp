#pragma once

#include "HttpHandler.hpp"

class AutoIndexHandler : public HttpHandler
{
  public:
    
    AutoIndexHandler(const Request& request, const Location& location, const ServerConfig& server);
    ~AutoIndexHandler();
    
  private:
  
    AutoIndexHandler& operator=(const AutoIndexHandler& other);
    AutoIndexHandler(const AutoIndexHandler& other);
    
};