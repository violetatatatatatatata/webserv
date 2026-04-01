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

        // Getters
        std::string getRoute() const;
        e_type      getType() const;
        int         getError() const;

        // Setters
        void setRoute();
        void setType();
        void setError();

    private:

        std::string _route;
        e_type      _type;
        int         _err_code;
};