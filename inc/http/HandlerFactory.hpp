#pragma once

#include <HandlerFactory.hpp>
#include <Request.hpp>
#include <RedirectHandler.hpp>
#include <CGIHandler.hpp>
#include <AutoIndexHandler.hpp>
#include <StaticHandler.hpp>
#include <ErrorHandler.hpp>

class HttpHandler;
class LocationParser;
class ServerParser;
class Request;
class Response;

class HandlerFactory
{
  public:

    static HttpHandler* create(const Request& request, const LocationParser* location, const ServerParser& server);
};
