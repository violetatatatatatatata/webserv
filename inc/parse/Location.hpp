/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:05:36 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/02 16:31:52 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> fileVector;

enum LocDir {
    LOC_ROOT,
    LOC_INDEX,
    LOC_AUTOINDEX,
    LOC_METHODS,
    LOC_UPLOAD_STORE,
    LOC_CGI_INFO,
	LOC_RETURN,
	LOC_UNEXPECTED
};

class ServerConfig;

/*	_autoindex: booleano que activa o desactiva el listado de directorios si no hay un index
 *	_index: archivo por defecto para esta ubicación específica
 *	_path: prefijo de la URL que define este bloque de configuración
 *	_root: directorio raíz específico para esta ubicación
 *	_upload_store: ruta del servidor donde se guardarán los archivos subidos
 *	_methods: lista de métodos HTTP permitidos (GET, POST, DELETE, etc.)
 *	_redirect: URL o ruta a la que se debe redirigir la petición
 *	_cgi_info: mapa que asocia extensiones de archivos con sus ejecutables CGI correspondientes
 *
 *	getAutoindex: comprueba si el autoindex está habilitado
 *	getPath: devuelve la ruta (path) de la ubicación
 *	getRoot: devuelve el directorio raíz de la ubicación
 *	getIndex: devuelve el archivo de índice de la ubicación
 *	getUploadStore: devuelve la ruta de almacenamiento de subidas
 *	getMethods: devuelve los métodos HTTP permitidos en esta ruta
 *	getRedirect: devuelve la configuración de redirección
 *	getCgiInfo: devuelve el mapa de configuración de CGI
 *
 *	setPath: establece la ruta de la ubicación
 *	setRoot: establece el directorio raíz específico
 *	setIndex: establece el archivo de índice específico
 *	setAutoindex: habilita o deshabilita el listado de directorios
 *	addMethod: añade un método HTTP a la lista de permitidos
 *	setRedirect: establece una regla de redirección
 *	setUploadStore: define dónde se guardarán los archivos subidos
 *	addCgiInfo: registra una extensión y su binario CGI asociado
 *
 *	parseLocation: analiza el contenido de un bloque location dentro del archivo de configuración
 * */
class Location {
		private:
			bool																_autoindex;
			std::string													_index; //TODO puede tener varios index
			std::string													_path;
			std::string													_root;
			std::string													_upload_store;
			std::string                         _redirect;
			std::vector<std::string>						_methods;
			std::map<std::string, std::string>	_cgi_info;

		public:
			Location();
			Location(const Location &obj);
			Location &operator=(const Location &obj);
			~Location();
		
			// getters
			bool																			getAutoindex() const;
			const std::string&												getPath() const;
			const std::string&												getRoot() const;
			const std::string&												getIndex() const;
			const std::string& 												getUploadStore() const;
			const std::string&  											getRedirect() const;
			const std::vector<std::string>& 					getMethods() const;
			const std::map<std::string, std::string>&	getCgiInfo() const;
			
			// setters
			void setPath(const std::string& path);
			void setRoot(const std::string& root);
			void setIndex(const std::string& index);
			void setAutoindex(bool state);
			void addMethod(const std::string& method);
			void setRedirect(const std::string& redirect);
			void setUploadStore(const std::string& path);
			void addCgiInfo(const std::string& ext, const std::string& bin);

			bool parseLocation(size_t &pos, const fileVector &file,
					ServerConfig &s, Location &loc);
};

#endif
