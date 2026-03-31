/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:59:13 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 14:47:29 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_HPP
# define READER_HPP

/*	aqui se carga TODO el file como un vector de strings (**file)
 * */
typedef std::vector<std::string> fileVector;

 #include <Webserv.hpp>

namespace Config {

class Reader {

	public:
		fileVector	readFile(const char *f);
		class FileNotFoundException: public std::exception {
			virtual const char *what() const throw();
		};
};
}

#endif
