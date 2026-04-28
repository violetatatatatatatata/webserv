#pragma once

#include <string>

class Location;
class Request;
class ServerConfig;
class Response;

class HttpHandler
{
    public:

        HttpHandler(const Request& request, const Location* location, const ServerConfig& server);
        HttpHandler(const HttpHandler& other);
        virtual ~HttpHandler();

        // Methods
        virtual void handleRequest(Response& response) = 0;

    protected:

        int isFileInError(int mode, const std::string file) const;

        const Request&      _request;
        const Location*     _location;
        const ServerConfig& _server;
};