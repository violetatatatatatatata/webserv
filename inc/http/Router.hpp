#pragma once

#include "Request.hpp"
#include "../Webserv.hpp"

enum e_type
{
    STATIC,
    CGI,
    DIRECTORY,
    ERROR
};

class Router
{
    public:

        Router();
        Router(const Router& other);
        ~Router();
        Router& operator=(const Router& other);

        // Methods
        void solveRoute(const Request& request);
        void findVirtualServer();
        void findLocation();

        // Getters
        std::string   getRoute() const;
        e_type        getType() const;
        int           getError() const;
        ServerConfig& getVirtualServer() const;
        Location&     getLocation() const;

        // Setters
        void setRoute(std::string route);
        void setType(e_type type);
        void setError(int error);

    private:

        const std::string&   _route;
        const ServerConfig&  _virtualServer;
        const Location&      _location;
        const Request&       _request;
        const e_type         _type;
        const int            _err_code;
};