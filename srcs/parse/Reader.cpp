/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:44:22 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/07 15:15:25 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>
#include <Reader.hpp>

Reader::Reader() {}

Reader::~Reader() {}

const char *Reader::FileNotFoundException::what() const throw() {
	return "Error: File could not be opened or does not exist.";
}

fileVector Reader::readFile(const char* f)
{
    size_t          i;
    size_t          hash_pos;
	fileVector      tokens;
	std::string     line;
	std::ifstream   file(f);

	if (!file.is_open()) {
	    throw FileNotFoundException();
		return tokens;
	}
	while (std::getline(file, line))
	{
		i = 0;
		hash_pos = line.find('#');
		if (hash_pos != std::string::npos)
			line.erase(hash_pos);
		while (i < line.length())
		{
			while (i < line.length() && isspace(line[i])) {
				i++;
			}
			if (i == line.length())
				break;
			if (line[i] == '{' || line[i] == '}' || line[i] == ';') {
				tokens.push_back(std::string(1, line[i]));
				i++;
			}
			else {
				size_t start = i;
				while (i < line.length() && !isspace(line[i]) &&
				       line[i] != '{' && line[i] != '}' && line[i] != ';') {
					i++;
				}
				tokens.push_back(line.substr(start, i - start));
			}
		}
	}
	file.close();
	return tokens;
}
