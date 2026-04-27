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

#include "Webserv.hpp"

int main(int argc, char **argv){

	if (argc != 2)
		return (print_msg("Uso: ./webserv [archivo.config]", FATAL));
	
	Config parser;
	if (parser.parseFile(argv[1]) == 0) {
		//print_msg("todo bien! :)", DEBUG);
		
		Cluster cluster(parser.getServers()); 
		cluster.run();
	}
	else
		return (print_msg("mu mal :(", DEBUG));
}