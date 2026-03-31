/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 ServerConfig.cpp									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: avelandr <avelandr@student.42barcelon		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/03/31 14:40:51 by avelandr		   #+#	  #+#			  */
/*	 Updated: 2026/04/01 00:22:37 by avelandr		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include <Webserv.hpp>

static int parseListen(int &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "listen");
    server.setPort(std::atoi(val.c_str()));
    return pos;
}

static int parseHost(int &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "host");
    server.setHost(val);
    return pos;
}

static int parseMaxSize(int &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "client_max_body_size");
    server.setMaxBodySize(std::atoll(val.c_str()));
    return pos;
}
static int parseServerName(int &pos, const fileVector &file, ServerConfig &server) {
	while (pos < file.size() && file[pos] != ";") {
		server.addServerName(file[pos]);
		pos++;
	}
	if (pos >= file.size() || file[pos] != ";")
		return (print_msg("server_name: missing semicolon", ERR));
	return pos;
}

static int parseRoot(int &pos, const fileVector &file, ServerConfig &server) {
	if (pos >= file.size() || file[pos] == ";")
		return (print_msg("root: missing value", ERR));
	server.setRoot(file[pos]);
	pos++;
	if (pos >= file.size() || file[pos] != ";")
		return (print_msg("root: missing semicolon", ERR));
	return pos;
}

static int parseErrorPage(int &pos, const fileVector &file, ServerConfig &server) {
	if (pos + 1 >= file.size())
		return (print_msg("error_page: invalid format", ERR));
	
	int code = std::atoi(file[pos].c_str());
	std::string path = file[pos + 1];
	server.addErrorPage(code, path);
	pos += 2;
	if (pos >= file.size() || file[pos] != ";")
		return (print_msg("error_page: missing semicolon", ERR));
	return pos;
}

static int redirectDictionary(std::string word)
{
	if (word == "listen")					return LISTEN;
	if (word == "host")						return HOST;
	if (word == "server_name")				return SERVER_NAME;
	if (word == "error_page")				return ERROR_PAGE;
	if (word == "client_max_body_size")		return MAX_BODY_SIZE;
	if (word == "root")						return ROOT;
	if (word == "index")					return INDEX;
	if (word == "location")					return LOCATION;
	return UNEXPECTED;
}

static int	redirectServer(int pos, fileVector file, ServerConfig &server, int serv)
{
	int	ret;

	pos++;
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
int ServerConfig::parseServer(int pos, fileVector file, ServerConfig &server)
{
	unsigned int	status;
	int				servEnum;

	while (pos < file.size() && file[pos] != "}") {
		servEnum = redirectDictionary(file[pos]);
		status = redirectServer(pos, file, server, servEnum);
		if (status == UNEXPECTED)
			return (print_msg("Unexpected character in server configuration!", ERR));
		pos++;
	}
	this->_serverNames.push_back(server.getHost());

	return (print_msg("Server parsed successfully :)", DEBUG));
}

/*	costructor
 *		1048576 bytes = 1MB por defecto, como en Nginx
 * */ 
ServerConfig::ServerConfig() : _port(0), _maxSize(1048576), _host("") {}

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
		this->_port			  = obj._port;
		this->_host			  = obj._host;
		this->_max_body_size  = obj._maxSize;
		this->_server_names   = obj._serverNames;
		this->_error_pages	  = obj._errPages;
		this->_locations	  = obj._locations;
		this->_root = obj._root;
	}
	return *this;
}

/*	destructor
 *		los destructores de vector, map y string estan implementados
 *		automaticamente por lo tanto no dan leaks por si mismos
 */
ServerConfig::~ServerConfig() {}

// getters

int ServerConfig::getPort() const {
	return _port;
}

size_t ServerConfig::getMaxBodySize() const {
	return _maxSize;
}

const std::string& ServerConfig::getHost() const {
	return _host; 
}

const std::vector<std::string>& ServerConfig::getServerNames() const {
	return _serverNames;
}

const std::map<int, std::string>& ServerConfig::getErrorPages() const {
	return _errPages;
}

const std::vector<Location>& ServerConfig::getLocations() const {
	return _locations;
}

const std::string& ServerConfig::getRoot() const {
	return _root;
}

// setters

void ServerConfig::setPort(int port) {
	_port = port;
}

void ServerConfig::setHost(std::string& host) {
	_host = host;
}

void ServerConfig::setMaxBodySize(size_t size) {
	_maxSize = size;
}

void ServerConfig::addServerName(std::string& name) {
	_serverNames.push_back(name);
}

void ServerConfig::addErrorPage(int code, std::string& path) {
	_errorPages[code] = path;
}

void ServerConfig::addLocation(Location& location) {
	_locations.push_back(location);
}

void ServerConfig::setRoot(const std::string& root) {
	_root = root;
}
