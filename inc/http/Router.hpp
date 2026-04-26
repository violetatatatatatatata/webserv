#pragma once

#include <vector>

class Request;
class Location;
class ServerConfig;
class Config;


class Router
{
    public:

        Router();
        ~Router();

        // Methods
        static const ServerConfig& findMatchingServer(const Request& request, const std::vector<ServerConfig>& servers);
        static const Location* findMatchingLocation(const Request& request, const ServerConfig& server);
};
