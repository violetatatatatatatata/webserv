/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 Config.cpp											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: avelandr <avelandr@student.42barcelon		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/03/30 16:17:35 by avelandr		   #+#	  #+#			  */
/*	 Updated: 2026/04/02 17:47:17 by avelandr		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include <Webserv.hpp>

/*	Implemented classes
 * */
static bool is_duplicated(const ServerConfig& s, std::vector<ServerConfig>& curr)
{
    int port = s.getPort();

    std::stringstream ss;
    ss << port;
    std::string s_port = ss.str();

    for (size_t j = 0; j < curr.size(); ++j)
    {   
        const std::vector<std::string>& names = s.getServerNames();
        const std::vector<std::string>& existing_names = curr[j].getServerNames();
            
        for (size_t n = 0; n < names.size(); ++n) {
            for (size_t m = 0; m < existing_names.size(); ++m) {
                if (names[n] == existing_names[m]) {
                    print_msg("Duplicate server_name '" + names[n]
                        + "' on port " + s_port, ERR);
                    return true;
                }   
            }   
        }   
    }
    return false;
}

int Config::parseFile(const char *f)
{
    fileVector file;
    Reader     r;

    file = r.readFile(f);
    if (file.empty())
        return (print_msg("Config file is empty or not found", ERR));

    size_t i = 0;
    while (i < file.size()) {
        if (file[i] == "server") {
            ServerConfig s;
            i++;
            if (i >= file.size() || file[i] != "{")
                return (print_msg("Expected '{' after server", ERR));
            i++;
            if (!s.parseServer(i, file, s))
				return -1;
			
			std::vector<ServerConfig>& curr = this->_servers[s.getPort()];
			if (is_duplicated(s, curr))
				return -1;
			
			this->_servers[s.getPort()].push_back(s);
        } else
            return (print_msg("Unexpected token in global scope: " + file[i], ERR));
        i++;
	}
    return EXIT_SUCCESS;
}

const ServerConfig& Config::findSpecificServer(const int port, const std::string& host) const
{
	const std::vector<ServerConfig>& specificPortServerList = _servers.find(port)->second;
	
	std::vector<ServerConfig>::const_iterator it;
	for (it = specificPortServerList.begin(); it < specificPortServerList.end(); it++)
	{
		if (it->hasServerName(host))
		{
			return *it;
		}
	}

	//assert(!specificPortServerList.empty());
	// If no match, return default server
	return specificPortServerList.at(0);
}

// OCF
Config::Config() {}
Config::~Config() {}

Reader::Reader() {}
Reader::~Reader() {}
