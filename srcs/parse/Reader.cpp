/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:44:22 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/07 14:38:48 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

/*	implemented functions
 * */

//	>> operador de extraccion de flujo
fileVector Reader::readFile(const char* f)
{
    std::ifstream file(f);
    fileVector tokens;
    std::string line;

    if (!file.is_open())
        return tokens;
    while (std::getline(file, line))
	{
        std::string spaced_line;
        for (size_t i = 0; i < line.length(); ++i) {
            if (line[i] == ';' || line[i] == '{' || line[i] == '}') {
                spaced_line += ' ';
                spaced_line += line[i];
                spaced_line += ' ';
            } else {
                spaced_line += line[i];
            }
        }
        std::istringstream iss(spaced_line);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
    }
    file.close();
    return tokens;
}
