#include "Router.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "ServerConfig.hpp"

Router::Router() {}
Router::~Router() {}

const ServerConfig& Router::findMatchingServer(const Request& request, const std::vector<ServerConfig>& servers)
{
  std::string host = request.getHeader("host");

  if (host.empty())
  {
    host = "default";
  }

	std::vector<ServerConfig>::const_iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
	{
		if (it->hasServerName(host))
		{
			return *it;
		}
	}

  //assert(!specificPortServerList.empty());
	
  // If no match, return default server
	return servers.at(0);
}

const Location* Router::findMatchingLocation(const Request& request, const ServerConfig& server)
{
  const Location* location = NULL;
  std::string curr_location_path;
  std::string prev_location_path;
  std::vector<Location>::const_iterator locations_it = server.getLocations().begin();

  for (; locations_it < server.getLocations().end(); locations_it++)
  {
    curr_location_path = locations_it->getPath();

    if (!request.getURI().compare(0, curr_location_path.size(), curr_location_path))
    {
      if (curr_location_path.size() > prev_location_path.size())
      {
        location = &(*locations_it);
      }
    }
    prev_location_path = curr_location_path;
  }
  
  return location;
}