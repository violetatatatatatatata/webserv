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

  // If no match, return default server
	return servers.at(0);
}

const Location* Router::findMatchingLocation(const Request& request, const ServerConfig& server)
{
    const Location* best = NULL;
    size_t best_len = 0;

    const std::vector<Location>& locations = server.getLocations();

    for (size_t i = 0; i < locations.size(); i++)
    {
        const std::string& path = locations[i].getPath();

        if (request.getURI().compare(0, path.size(), path) == 0)
        {
            if (path.size() > best_len)
            {
                best = &locations[i];
                best_len = path.size();
            }
        }
    }

    return best;
}