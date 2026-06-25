#pragma once

#include <string>

class LocationParser;
class Request;
class ServerParser;
class Response;

class HttpHandler
{
    public:

        HttpHandler(const Request& request, const LocationParser* location, const ServerParser& server);
        HttpHandler(const HttpHandler& other);
        virtual ~HttpHandler();

        // Methods
        virtual void handleRequest(Response& response) = 0;
        static int isFileInError(int mode, const std::string file);

    protected:


        const Request&          _request;
        const LocationParser*   _location;
        const ServerParser&     _server;
};