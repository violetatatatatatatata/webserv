/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:44:22 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/01 01:39:21 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

/*	implemented functions
 * */

//	>> operador de extraccion de flujo
fileVector Reader::readFile(const char* f) {
        std::ifstream file(f);
        fileVector tokens;
        std::string token;

        //if (!file.is_open())
		//	return (print_msg("Cannot open config file!", FATAL)); No es el bueno tipo de return (fileVector)
        while (file >> token)
            tokens.push_back(token);
        file.close();
        return tokens;
}

