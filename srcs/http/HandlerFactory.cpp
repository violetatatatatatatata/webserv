#include "HandlerFactory.hpp"
#include "ServerConfig.hpp"
#include "Location.hpp"
#include "StaticHandler.hpp"
#include "CGIHandler.hpp"
#include "RedirectHandler.hpp"
#include "AutoIndexHandler.hpp"

HttpHandler* HandlerFactory::create(const Request& request, const Location* location, const ServerConfig& server)
{
  HttpHandler* handler = NULL;
  
  if (!location && !server.getRoot().empty())
    return new StaticHandler(request, location, server);
  
  if (location)
  {
    switch (location->getLocType())
    {
      case STATIC_TYPE: return new StaticHandler(request, location, server);
      case CGI_TYPE: return new CGIHandler(request, location, server);
      case REDIRECT_TYPE: return new RedirectHandler(request, location, server);
      case AUTOINDEX_TYPE: return new AutoIndexHandler(request, location, server);
    }
  }

  return handler;
}
