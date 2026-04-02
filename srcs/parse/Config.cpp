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

int	Config::parseFile(const char *f)
{
	size_t			i = 0;
	Reader	   r; 
	fileVector		file;

	file = r.readFile(f);
	if(file.empty())
		return (print_msg("Config file is empty or not found", ERR));

	while (i < file.size()) {
		if (file[i] == "server") {
			ServerConfig s;
			if (file[++i] != "{")
				return (print_msg("Expected '{' after server directive", ERR));
			if (!s.parseServer(++i, file, s))
				return (print_msg("Error in config file", ERR));
		}
		else
			return (print_msg("Unknown directive", ERR));
		i++;
	}
	return (print_msg("File parsed successfully :)", DEBUG));
}

