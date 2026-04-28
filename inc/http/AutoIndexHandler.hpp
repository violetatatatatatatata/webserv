#pragma once

#include "HttpHandler.hpp"

class AutoIndexHandler : public HttpHandler
{
  public:
    
    AutoIndexHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& directory);
    ~AutoIndexHandler();
    
    // Methods
    void handleRequest(Response& response);

  private:
  
    AutoIndexHandler(const AutoIndexHandler& other);

    std::string _directory;
    
};