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
#include "Config.hpp"
#include "Location.hpp"
#include <iostream>

int main(int argc, char **argv){

	if (argc != 2)
		return (print_msg("Uso: ./webserv [archivo.config]", FATAL));
	
	Config parser;
	if (parser.parseFile(argv[1]) == 0) {
		print_msg("todo bien! :)", DEBUG);
	
		Cluster cluster(parser.getServers()); 
		cluster.run();
	
	}
	else
		return (print_msg("mu mal :(", DEBUG));
	       
    std::string str = "GET /index.html HTTP/1.1\r\n"
"Host: localhost:8080\r\n"
"User-Agent: WebservTest/1.0\r\n"
"Accept: text/html\r\n"
"Content-Length: 13\r\n"
"\r\n"
"Hello world!";
    Request request(str);
    Response response;
    const ServerConfig& server = Router::findMatchingServer(request, parser);
    const Location* location = Router::findMatchingLocation(request, server);

    HttpHandler* const handler = HandlerFactory::create(request, location, server);
    

    if (handler)
        handler->handleRequest(response);
    else
        std::cout << "NULL" << std::endl;
    std::cout << response.buildResponse() << std::endl;

}
