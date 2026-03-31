/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:05:36 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 18:05:40 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

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

enum loc {
	PATH,
	METHODS,
	ROOT,
	INDEX,
	AUTOINDEX,
	UPLOAD_STORE,
	CGI,
	RETURN
}

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

			int	parseLocation(int pos, fileVector file);
};

#endif
