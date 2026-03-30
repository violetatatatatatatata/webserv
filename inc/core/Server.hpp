/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 Server.hpp											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: avelandr <avelandr@student.42barcelon		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/03/30 12:03:47 by avelandr		   #+#	  #+#			  */
/*	 Updated: 2026/03/30 12:16:42 by avelandr		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#define

#include <>

namespace Config {
/*	port: puerto de escucha
 *	max_size: maximo tamaño del body cuando este es fijo
 *	host: ip del host
 *	server_names: lista de nombres
 *	err_pages: mapa de paginas html que se acceden mediante la id del error
 *	locations: bloque de locations
 * */
class Server {
		private:
			int							_port;
			size_t						_max_body_size;
			std::string					_host;
			std::vector<std::string>	_server_names;
			std::map<int, std::string>	_err_pages;
			std::vector<Location>		_locations;
		
		public:
			Server();
			Server(const Server &obj);
			Server &operator=(const Server &obj);
			~Server();

			// getters
			int							getPort();
			size_t						getMaxBodySize();
			std::string&				getHost();
			std::vector<std::string>&	getServerNames();
			std::map<int, std::string>&	getErrorPages();
			std::vector<Location>&		getLocations();
			
			// setters
			void	setPort(int port);
			void	setHost(std::string& host);
			void	setMaxBodySize(size_t size);
			void	addServerName(std::string& name);
			void	addErrorPage(int code, std::string& path);
			void	addLocation(Location& location);
	};

/*	autoindex: permite mirar si index.html existe o no. Si no existe, generar un html con la lista de archivos
 *	index: indica la respuesta predeterminada si la url acaba en /
 *	path: prefijo de la url 
 *	root: ruta absoluta o relativa en el disco duro local
 *	upload_store: la ruta en el servidor para los archivos de los metodos POST o PUT
 *	methods: lista de palabras permitidas (GET, POST, ...)
 *	cgi_info: mapa que usa el servidor para saber qué script ejecutar
 *	  key: la extensión del archivo (ej. ".php", ".py", ".pl").
	  value: la ruta absoluta al ejecutable (ej. "/usr/bin/php-cgi", "/usr/bin/python3").
 * */
class Location {
		private:
			bool								_autoindex;
			std::string							_index;
			std::string							_path;
			std::string							_root;
			std::string							_upload_store;
			std::vector<std::string>			_methods;
			std::map<std::string, std::string>	_cgi_info;

		public:
			Location();
			Location(const Location &obj);
			Location &operator=(const Location &obj);
			~Location();
		
			// getters
			bool								getAutoindex()
			std::string&						getPath();
			std::string&						getRoot();
			std::string&						getIndex();
			std::string& 						getUploadStore();
			std::vector<std::string>& 			getMethods();
			std::map<std::string, std::string>&	getCgiInfo();
			
			// setters
			void setPath(std::string& path);
			void setRoot(std::string& root);
			void setIndex(std::string& index);
			void setAutoindex(bool state);
			void addMethod(std::string& method);
			void setUploadStore(std::string& path);
			void addCgiInfo(std::string& ext,: std::string& bin);
};
}

#endif
