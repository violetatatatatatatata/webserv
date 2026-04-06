#include "../inc/http/Router.hpp"

Router::Router()
{

}

Router::Router(const Router& other)
{

}

Router::~Router()
{

}

Router& Router::operator=(const Router& other)
{

}

// Getters

std::string Router::getRoute() const
{
  return _route;
}

e_type Router::getType() const
{
  return _type;
}

int Router::getError() const
{
  return _error;
}

ServerConfig& Router::getVirtualServer() const
{
  return _virtualServer;
}

Location& Router::getLocation() const
{
  return _location;
}

// Setter

void Router::setRoute(std::string route)
{
  _route = route;
}

void Router::setType(e_type type)
{
  _type = type;
}

void Router::setError(int error)
{
  _error = error;
}

// Methods

void Router::solveRoute(const Request& request)
{

}

void Router::findVirtualServer()
{
  std::map<int, std::vector<ServerConfig>> servers = Config::getServers();
}

void Router::findLocation()
{
}