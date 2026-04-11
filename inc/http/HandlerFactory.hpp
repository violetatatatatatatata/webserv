#pragma once

class HttpHandler;
class Location;
class ServerConfig;
class Request;

class HandlerFactory
{
  public:
  
    static HttpHandler* create(const Request& request, const Location* location, const ServerConfig& server);
};
