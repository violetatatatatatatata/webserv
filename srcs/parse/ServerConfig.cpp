/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:40:51 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 18:43:24 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

namespace Config {

static int	parseListen(int pos, fileVector file, ServerConfig server) {

	// ...
}

static int	parseServerName(int pos, fileVector file, ServerConfig server) {

	// ...
}

static int	parseRoot(int pos, fileVector file, ServerConfig server) {

	// ...
}

static int	parseIndex(int pos, fileVector file, ServerConfig server) {

	// ...
}

static int redirectDictionary(str::string word)
{
	enum Serv	ret;

	if		(word == "listen")
		ret = LISTEN;
	else if (word == "host")
		ret = HOST;
	else if (word == "server_name")
	
	// ...
}


static int	redirectServer(int pos, fileVector file, ServerConfig server, int serv)
{
	int	ret;

	switch (serv) {
		case LISTEN:
			ret = parseListen(pos, file, server);		break;
		case HOST:
			ret = parseHost(pos, file, server);			break;
		case SERVER_NAME:
			ret = parseServerName(pos, file, server);	break;
		case ERROR_PAGE:
			ret = parseErrorPage(pos, file, server);	break;
		case MAX_BODY_SIZE:
			ret = parseMaxSize(pos, file, server);		break;
		case ROOT:
			ret = parseRoot(pos, file, server);			break;
		case INDEX:
			ret = parseIndex(pos, file, server);		break;
		case LOCATION:
			ret = parseLocation(pos, file);				break;
		default:
			ret = UNEXPECTED;
	}
	return (ret);
}	


/*	implemented functions
 * */
int	parseServer(int pos, fileVector file, ServerConfig server)
{
	unsigned int	status;

	while (pos++ < file.size() && file[pos] != '}') {
		status = redirectServer(pos, file, server);
		if (status == UNEXPECTED)
			return (print_msg("Unexpected character in server configuration!", ERR));
	}
	this->_serverNames.push_back(server);

	return (print_msg("Server parsed successfully :)", DEBUG));
}

/*	costructor
 *		1048576 bytes = 1MB por defecto, como en Nginx
 * */ 
ServerConfig::ServerConfig() : _port(0), _host(""), _max_body_size(1048576) {}

/*	copy constructor
 *		crea un nuevo server con exactamente los dato de otro ya existente
 * */
ServerConfig::ServerConfig(const ServerConfig &obj) {
    *this = obj;
}

/*	operator overload
 *		iguala un servidor a otro evitando igualar servers iguales
 *		por defecto, c++ copia todos los mapas, vectores y strings
 *		automáticamente
 * */
ServerConfig &ServerConfig::operator=(const ServerConfig &obj) {
    if (this != &obj) { 
        this->_port           = obj._port;
        this->_host           = obj._host;
        this->_max_body_size  = obj._max_body_size;
        this->_server_names   = obj._server_names;
        this->_error_pages    = obj._error_pages;
        this->_locations      = obj._locations;
    }
    return *this;
}

/*	destructor
 *		los destructores de vector, map y string estan implementados
 *		automaticamente por lo tanto no dan leaks por si mismos
 */
ServerConfig::~ServerConfig() {}

// getters

int ServerConfig::getPort() {
	return _port;
}

size_t ServerConfig::getMaxBodySize() {
	return _max_body_size;
}

std::string& ServerConfig::getHost() {
	return _host;
}

std::vector<std::string>& ServerConfig::getServerNames() {
	return _server_names;
}

std::map<int, std::string>& ServerConfig::getErrorPages() {
	return _error_pages;
}

std::vector<Location>& ServerConfig::getLocations() {
	return _locations;
}

// setters

void ServerConfig::setPort(int port) {
	_port = port;
}

void ServerConfig::setHost(std::string& host) {
	_host = host;
}

void ServerConfig::setMaxBodySize(size_t size) {
	_max_body_size = size;
}

void ServerConfig::addServerName(std::string& name) {
	_server_names.push_back(name);
}

void ServerConfig::addErrorPage(int code, std::string& path) {
	_error_pages[code] = path;
}

void ServerConfig::addLocation(Location& location) {
	_locations.push_back(location);
}

}
