/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:01:18 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/02 16:08:54 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <Webserv.hpp>
# include <Location.hpp>

enum Serv {
	LISTEN,
	HOST,
	SERVER_NAME,
	ERROR_PAGE,
	MAX_BODY_SIZE,
	ROOT,
	INDEX,
	LOCATION,
	UNEXPECTED
};

/*	port: puerto de escucha
 *	max_size: maximo tamaño del body cuando este es fijo
 *	host: ip del host
 *	server_names: lista de nombres
 *	err_pages: mapa de paginas html que se acceden mediante la id del error
 *	locations: bloque de locations
 *	servers: mapa con todos los puertos y los servidores asociados a este
 * */
class ServerConfig {
		private:
			int											_port;
			size_t										_max_body_size;
			std::string									_host;
			std::string									_root;
			std::string									_index;
			std::vector<std::string>					_server_names;
			std::map<int, std::string>					_error_pages;
			std::vector<Location>						_locations;
			std::map<int, std::vector<ServerConfig> >	_servers;
		
		public:
			ServerConfig();
			ServerConfig(const ServerConfig &obj);
			ServerConfig &operator=(const ServerConfig &obj);
			~ServerConfig();

			// getters
			int									getPort() const;
			size_t								getMaxBodySize() const;
			const std::string&					getHost() const;
			const std::string&					getRoot() const;
			const std::string&					getIndex() const;
			const std::vector<std::string>&		getServerNames() const;
			const std::map<int, std::string>&	getErrorPages() const;
			const std::vector<Location>&		getLocations() const;
			
			// setters
			void	setPort(int port);
			void	setHost(std::string& host);
			void	setRoot(const std::string& root);
			void	setMaxBodySize(size_t size);
			void	addServerName(const std::string& name);
			void	setIndex(const std::string& index);
			void	addErrorPage(int code, const std::string& path);
			void	addLocation(const Location& location);

			int	parseFile(const char *f);
			int parseServer(size_t pos, fileVector file,
					ServerConfig &server);
};

#endif
