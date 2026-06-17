/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 17:14:31 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/26 13:48:30 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>
#include <Config.hpp>
#include <Cluster.hpp>

int main(int argc, char **argv)
{
	if (argc != 2)
		return (print_msg("Uso: ./webserv [archivo.config]", FATAL));
	Config parser;
	if (parser.parseFile(argv[1]) == 0) {
		Cluster cluster(parser.getServers());
		cluster.run();
	}
	else
		return (print_msg("Error parsing config file. Please check and try again!", ERR));
}
