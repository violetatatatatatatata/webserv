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
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2)
        return (print_msg("Uso: ./webserv [archivo.config]", FATAL));
    Config parser;
    if (parser.parseFile(argv[1]) == 0) {
        std::cout << "Configuración válida" << std::endl;
        
        std::vector<ServerConfig> servers = parser.getServers();
        for (size_t i = 0; i < servers.size(); ++i) {
            std::cout << "Server " << i << ": " << servers[i].getHost() 
                      << ":" << servers[i].getPort() << std::endl;
        }
    } else {
        std::cout << "Error en la configuración" << std::endl;
        return 1;
    }
    return 0;
}
