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

 #include <Webserv.hpp>

// #define

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
			int							getPort() const;
			size_t						getMaxBodySize() const;
			const std::string&				getHost() const;
			const std::vector<std::string>&	getServerNames() const;
			const std::map<int, std::string>&	getErrorPages() const;
			const std::vector<Location>&		getLocations() const;
			
			// setters
			void	setPort(int port);
			void	setHost(std::string& host);
			void	setMaxBodySize(size_t size);
			void	addServerName(const std::string& name);
			void	addErrorPage(int code, const std::string& path);
			void	addLocation(const Location& location);
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
			bool										getAutoindex()
			const std::string&							getPath() const;
			const std::string&							getRoot() const;
			const std::string&							getIndex() const;
			const std::string& 							getUploadStore() const;
			const std::vector<std::string>& 			getMethods() const;
			const std::map<std::string, std::string>&	getCgiInfo() const;
			
			// setters
			void setPath(const std::string& path);
			void setRoot(const std::string& root);
			void setIndex(const std::string& index);
			void setAutoindex(bool state);
			void addMethod(const std::string& method);
			void setUploadStore(const std::string& path);
			void addCgiInfo(const std::string& ext, const std::string& bin);
};
}

#endif
