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
Config::Config() {}
Config::~Config() {}

Reader::Reader() {}
Reader::~Reader() {}

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
            this->_servers.push_back(s);
        } else
            return (print_msg("Unexpected token in global scope: " + file[i], ERR));
        i++;
	}
    return EXIT_SUCCESS;
}
