/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:17:35 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 18:43:41 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

/*	Implemented classes
 * */
int	Config::parseFile(const char *f)
{
	int				i = -1;
	fileVector		file;

	file = Reader::readFile(f);

	while (++i < file.size()) {
		if (file[i] == "server") {
			ConfigServer server;
			if (file[++i] != "{")
				return (print_msg("Expected '{' after server directive", ERR));
			if (!server.parseServer(++i, file, server))
				return (print_msg("Error in config file", ERR));
		}
		else
			return (print_msg("Unknown directive", ERR));
	}
	return (print_msg("File parsed successfully :)", DEBUG));
}
