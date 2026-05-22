#pragma once

#include "HttpHandler.hpp"
#include <vector>

class ServerParser;
class Response;


struct ParsedURL
{
    std::string directory;
    std::string script;
    std::string path;
};

class CGIHandler : public HttpHandler
{
  public:
    CGIHandler(const Request& request, const LocationParser* location, const ServerParser& server, const std::string& scriptPath, const std::string& extention);
    ~CGIHandler();

    // Methods
    void handleRequest(Response& response);
    
  private:
  
    CGIHandler(const CGIHandler& other);

    ParsedURL   parseURL() const;
    std::string findScript() const;
    void        internalError(const std::string& msg, Response& response) const;
    void        findExtention();
    void        executeCGI() const;
    char**      buildArgv(const std::string& script) const;
    char**      castEnv(const ParsedURL& urlInfo) const;
    void        handleFd(int fd[2], Response& response) const;
    std::string getDir() const; 
    std::vector<std::string> buildEnv(const ParsedURL& urlInfo) const;

    std::string _url;
    std::string _script;
    std::string _binPath;
    std::string _binName;
    std::string _ext;
};