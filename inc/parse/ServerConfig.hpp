/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:01:18 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/09 12:07:37 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <Webserv.hpp>

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

/*	_port: puerto en el que el servidor escuchará las conexiones
 *	_max_body_size: límite máximo permitido para el cuerpo de las peticiones del cliente
 *	_host: dirección IP o nombre de host asociado al servidor
 *	_root: directorio raíz desde donde se servirán los archivos
 *	_index: archivo por defecto que se servirá al acceder a un directorio
 *	_server_names: lista de nombres (dominios) que identifican al servidor
 *	_error_pages: mapa que asocia códigos de error HTTP con rutas a páginas personalizadas
 *	_locations: lista de bloques de configuración específicos para rutas (URIs)
 *
 *	getPort: devuelve el puerto de escucha
 *	getMaxBodySize: devuelve el tamaño máximo del cuerpo de la petición
 *	getHost: devuelve la dirección IP/host
 *	getRoot: devuelve la ruta raíz del servidor
 *	getIndex: devuelve el archivo de índice configurado
 *	getServerNames: devuelve la lista de nombres del servidor
 *	getErrorPages: devuelve el mapa de páginas de error
 *	getLocations: devuelve el vector con todas las rutas configuradas
 *
 *	setPort: establece el puerto del servidor
 *	setHost: establece la dirección IP del servidor
 *	setRoot: establece el directorio raíz del servidor
 *	setMaxBodySize: define el tamaño máximo del body permitido
 *	addServerName: añade un nuevo nombre a la lista de nombres del servidor
 *	setIndex: establece el archivo de índice por defecto
 *	addErrorPage: añade una ruta de página personalizada para un código de error específico
 *	addLocation: añade un bloque de configuración de ruta (Location) al servidor
 *
 *	parseFile: inicia el procesamiento de un archivo de configuración
 *	parseServer: analiza un bloque de servidor específico dentro del archivo
 * */
class ServerConfig {
		private:
			int											    _port;
			size_t									    _max_body_size;
			std::string							    _host;
			std::string									_root;
			std::string									_index; // TODO puede tener varios index
																					//falta redirection, methods
			std::vector<std::string>		_server_names;
			std::map<int, std::string>  _error_pages;
			std::vector<Location>				_locations;
		
		public:
			ServerConfig(int port); //to delete
			ServerConfig();
			ServerConfig(const ServerConfig &obj);
			ServerConfig &operator=(const ServerConfig &obj);
			~ServerConfig();

			// getters
			int									              getPort() const;
			size_t								            getMaxBodySize() const;
			const std::string&				      	getHost() const;
			const std::string&					      getRoot() const;
			const std::string&					      getIndex() const;
			const std::vector<std::string>&		getServerNames() const;
			const std::map<int, std::string>&	getErrorPages() const;
			const std::vector<Location>&		  getLocations() const;
			
			// setters
			void	setPort(int port);
			void	setHost(const std::string& host);
			void	setRoot(const std::string& root);
			void	setMaxBodySize(size_t size);
			void	addServerName(const std::string& name);
			void	setIndex(const std::string& index);
			void	addErrorPage(int code, const std::string& path);
			void	addLocation(const Location& location);

      // methods
			int	 parseFile(const char *f);
      bool hasServerName(const std::string& server_name) const;
			bool parseServer(size_t &pos, const fileVector &file,
					ServerConfig &server);
};

#endif
