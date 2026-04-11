/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 17:14:31 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/07 14:24:48 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Config.hpp"
#include "Location.hpp"
#include <iostream>

int main(int argc, char **argv){

    if (argc != 2)
        return (print_msg("Uso: ./webserv [archivo.config]", FATAL));
    
    Config parser;
    if (parser.parseFile(argv[1]) == 0)
    	print_msg("todo bien! :)", DEBUG);
    else
    	return (print_msg("mu mal :(", DEBUG));
	       
    Request request(8080);
    const ServerConfig& server = Router::findMatchingServer(request, parser);
    const Location* location = Router::findMatchingLocation(request, server);

    HttpHandler* const handler = HandlerFactory::create(request, location, server);
    
    if (handler)
        handler->resolveRequest();
    else
        std::cout << "NULL" << std::endl;

    delete(handler);
    (void)handler;
}
