/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:59:13 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/02 17:46:43 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_HPP
# define READER_HPP

 #include <Webserv.hpp>

class Reader {

	public:
		Reader();
        ~Reader();
		fileVector readFile(const char* f);
		class FileNotFoundException: public std::exception {
			virtual const char *what() const throw();
		};
};

#endif
