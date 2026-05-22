#pragma once

#include <Router.hpp>
#include <Request.hpp>
#include <Response.hpp>
#include <ServerParser.hpp>
#include <LocationParser.hpp>
#include <HandlerFactory.hpp>
#include <HttpHandler.hpp>

class Request;
class LocationParser;
class ServerParser;
class Config;


class Router
{
    public:

        Router();
        ~Router();

        // Methods
        static const ServerParser& findMatchingServer(const Request& request, const std::vector<ServerParser>& servers);
        static const LocationParser* findMatchingLocation(const Request& request, const ServerParser& server);
};
