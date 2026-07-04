#include <Router.hpp>

Router::Router() {}
Router::~Router() {}

const ServerParser& Router::findMatchingServer(const Request& request, const std::vector<ServerParser>& servers)
{
  std::string host = request.getHeader("host");

  if (host.empty())
  {
    host = "default";
  }

	std::vector<ServerParser>::const_iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
	{
		if (it->hasServerName(host))
		{
			return *it;
		}
	}

	return servers.at(0);
}

const LocationParser* Router::findMatchingLocation(const Request& request, const ServerParser& server)
{
    const LocationParser* best = NULL;
    size_t best_len = 0;
    const std::string& uri = request.getURI();
    const std::vector<LocationParser>& locations = server.getLocations();

    for (size_t i = 0; i < locations.size(); i++)
    {
        const std::string& path = locations[i].getPath();

        if (path.size() > uri.size())
            continue;

        bool prefixMatch = uri.compare(0, path.size(), path) == 0;
        bool boundaryOk = (path.size() == uri.size()) || (path[path.size() - 1] == '/') || (uri[path.size()] == '/');

        if (prefixMatch && boundaryOk && path.size() > best_len)
        {
            best = &locations[i];
            best_len = path.size();
        }
    }
    return best;
}

/*const LocationParser* Router::findMatchingLocation(const Request& request, const ServerParser& server)
{
    const LocationParser* best = NULL;
    size_t best_len = 0;

    const std::vector<LocationParser>& locations = server.getLocations();

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
}*/
