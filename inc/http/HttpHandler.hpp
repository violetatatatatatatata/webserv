#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

class Location;
class Request;
class ServerConfig;

class HttpHandler
{
    public:

        HttpHandler(const Request& request, const Location* location, const ServerConfig& server);
        virtual ~HttpHandler();

        // Methods
        virtual void resolveRequest() = 0;

    private:
        
        HttpHandler(const HttpHandler& other);
        HttpHandler& operator=(const HttpHandler& other);

    protected:

        const Request&      _request;
        const Location*     _location;
        const ServerConfig& _server;
};