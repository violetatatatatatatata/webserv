#pragma once

#include "Location.hpp"
#include "Request.hpp"
//#include "ServerConfig.hpp"
#include <string>

class ServerConfig;

class HttpHandler
{
    public:

        HttpHandler(const Request& request, const Location& location, const ServerConfig& server);
        virtual ~HttpHandler();
        // Methods
        bool checkIfFileExist() const;
        void resolvePath();

    private:
        
        HttpHandler(const HttpHandler& other);
        HttpHandler& operator=(const HttpHandler& other);

    protected:

        const ServerConfig& _server;
        const Location&     _location;
        const Request&      _request;

        std::string _absolute_path;
};