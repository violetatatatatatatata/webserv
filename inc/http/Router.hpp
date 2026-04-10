#pragma once

#include "Request.hpp"
#include "../parse/Config.hpp"
#include "../parse/ServerConfig.hpp"
#include "../parse/Location.hpp"

class Router
{
    public:

        Router();
        ~Router();

        // Methods
        static const ServerConfig& findMatchingServer(const Request& request, const Config& config);
        static const Location* findMatchingLocation(const Request& request, const ServerConfig& server);
};
