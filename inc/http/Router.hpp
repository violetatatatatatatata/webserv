#pragma once

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
        static const ServerConfig& findMatchingServer(const Request& request, const Config& config);
        static const Location* findMatchingLocation(const Request& request, const ServerConfig& server);
};
