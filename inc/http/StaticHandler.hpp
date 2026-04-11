#pragma once

#include "HttpHandler.hpp"

class StaticHandler : public HttpHandler
{
  public:
    
    StaticHandler(const Request& request, const Location* location, const ServerConfig& server);
    ~StaticHandler();

    void resolveAbsolutePath();
    void resolveRequest();
    void resolveGET() const;
    void resolvePOST() const;
    void resolveDELETE() const;

  private:
    
    StaticHandler(const StaticHandler& other);
    StaticHandler& operator=(const StaticHandler& other);

    bool isFileExistant() const;
    bool isMethodAuthorized() const;
    bool isFileReadable() const;

    std::string _absolute_path;
};