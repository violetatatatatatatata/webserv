#include "HandlerFactory.hpp"

const HttpHandler* HandlerFactory::create(const Request& request, const Location& location, const ServerConfig& server)
{
  HttpHandler* handler = NULL;
  
  switch (location.getLocType())
  {
    case STATIC_TYPE:
      return new HttpHandler(request, location, server);
      break ;
    case CGI_TYPE:
      return new CGIHandler(request, location, server);
      break ;
    case REDIRECT_TYPE:
      return new RedirectHandler(request, location, server);
      break ;
    case AUTOINDEX_TYPE:
      return new AutoIndexHandler(request, location, server);
      break ;
  }

  return handler;
}
