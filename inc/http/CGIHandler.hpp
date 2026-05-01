#pragma once

#include "HttpHandler.hpp"
#include <vector>

class ServerConfig;
class Response;

class CGIHandler : public HttpHandler
{
  public:
    CGIHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& extention);
    ~CGIHandler();

    // Methods
    void handleRequest(Response& response);
    
  private:
  
    CGIHandler(const CGIHandler& other);

    void    internalError(const std::string& msg, Response& response) const;
    void    findExtention();
    void    executeCGI() const;
    char**  buildArgv() const;
    char**  castEnv() const;
    std::string   getDir() const; 
    std::vector<std::string> buildEnv() const;

    std::string _scriptPath;
    std::string _binPath;
    std::string _binName;
    std::string _ext;
};