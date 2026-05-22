/* ************************************************************************** */

/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:15:33 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 10:16:14 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <Webserv.hpp>
# include <ServerParser.hpp>
# include <LocationParser.hpp>
# include <Reader.hpp>

/*
 *	_servers: map to organize server configurations by port
 *
 *	Config: default constructor
 *	Config (copy): copy constructor
 *	~Config: class destructor
 *	operator=: assignment operator overload
 *
 *	parseFile: reads and processes the full configuration file to fill the servers map
 *	getServers: returns the complete structure of all configured servers
 * */
class Config {
	private:
		std::map<int, std::vector<ServerParser> >	_servers;

	public:
		Config();
		Config(Config const &src);
		virtual ~Config();
		Config &operator=(Config const &src);

		int parseFile(const char *f);

		std::map<int, std::vector<ServerParser> > getServers() const { return _servers; };
};

#endif
