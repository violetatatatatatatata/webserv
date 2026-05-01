#pragma once

#include "HttpHandler.hpp"
#include <vector>

class ServerConfig;

class CGIHandler : public HttpHandler
{
  public:
    CGIHandler(const Request& request, const Location* location, const ServerConfig& server);
    ~CGIHandler();

    // Methods
    void handleRequest(Response& response);
    
  private:
  
    CGIHandler(const CGIHandler& other);

    void internalError(const std::string& msg) const;
    char**  buildBinary();
    char**  buildEnv();
    void    findExtention();

    std::vector<std::string>  _bin;
    std::vector<std::string>  _env;
    std::string               _ext;
};