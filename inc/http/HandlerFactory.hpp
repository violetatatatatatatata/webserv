#pragma once

#include "../parse/Location.hpp"
#include "../parse/ServerConfig.hpp"
#include "HttpHandler.hpp"

class HandlerFactory
{
  public:
  
    static const HttpHandler* create(const Request& request, const Location& location, const ServerConfig& server);
};
