#pragma once

class Location;
class Request;
class ServerConfig;
class Response;

class HttpHandler
{
    public:

        HttpHandler(const Request& request, const Location* location, const ServerConfig& server);
        virtual ~HttpHandler();

        // Methods
        virtual void handleRequest(Response& response) = 0;

    private:
        
        HttpHandler(const HttpHandler& other);
        HttpHandler& operator=(const HttpHandler& other);

    protected:

        const Request&      _request;
        const Location*     _location;
        const ServerConfig& _server;
};