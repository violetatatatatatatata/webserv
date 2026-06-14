/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:01:18 by avelandr          #+#    #+#             */
/*   Updated: 2026/06/14 13:35:38 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPARSER_HPP
# define SERVERPARSER_HPP

# include <Parser.hpp>
# include <LocationParser.hpp>

/*
 *	_port: port on which the server will listen
 *	_max_body_size: maximum allowed byte size for client request bodies
 *	_host: IP address or hostname where the server is bound
 *	_root: default root directory at the server level
 *	_index: default file to be served if a directory is requested
 *	_server_names: list of domain names or virtual hosts associated with the server
 *	_locations: vector containing all specific route configurations for the server
 *
 *	ServerParser: default constructor
 *	ServerParser (copy): copy constructor
 *	~ServerParser: class destructor
 *	operator=: assignment operator overload
 *
 *	initMap: instantiates and registers parsing actions for allowed directives in the server block
 *	parseServer: processes server tokens and validates its content
 *	hasServerName: checks if the current server responds to a specific domain name
 *	getPort: returns the configured port
 *	getMaxBodySize: returns the maximum body size limit
 *	getHost: returns the configured IP or host
 *	getRoot: returns the global root path
 *	getIndex: returns the global default index file
 *	getServerNames: returns the list of server domains
 *	getLocations: returns all nested location blocks
 *	addLocation: adds a newly parsed location block to the server's list
 * */
class ServerParser : public Parser {
	private:
		int									_port;
		size_t								_max_body_size;
		std::string							_host;
		std::string							_root;
		std::string							_index;
		std::vector<std::string>			_server_names;
		std::vector<LocationParser>			_locations;

	public:
		ServerParser();
		ServerParser(const ServerParser &obj);
		ServerParser &operator=(const ServerParser &obj);
		virtual ~ServerParser();

		virtual void initMap();

		// temporalmente para no romper nada
		bool parseServer(size_t &pos, const fileVector &file);
		bool hasServerName(const std::string& server_name) const;

		int									getPort() const;
		size_t								getMaxBodySize() const;
		const std::string&					getHost() const;
		const std::string&					getRoot() const;
		const std::string&					getIndex() const;
		const std::vector<std::string>&		getServerNames() const;
		const std::vector<LocationParser>&	getLocations() const;

		void addLocation(const LocationParser& location);
};

#endif
