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

static int parseListen(size_t &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "listen");
    if (val.empty())
		return -1;
    server.setPort(std::atoi(val.c_str()));
    return pos;
}

static int parseHost(size_t &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "host");
    if (val.empty())
		return -1;
    server.setHost(val);
    return pos;
}

static int parseIndex(size_t &pos, const fileVector &file, ServerConfig &serv) {
    std::string val = getDirectiveValue(pos, file, "index");
    if (val.empty())
		return -1;
    serv.setIndex(val);
    return pos;
}

static int parseMaxSize(size_t &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "client_max_body_size");
    if (val.empty())
		return -1;
    std::stringstream ss(val);
    size_t size;
    if (!(ss >> size) || !ss.eof()) {
        print_msg("client_max_body_size: invalid number format", ERR);
        return -1;
    }
    server.setMaxBodySize(size);
    return pos;
}

static int parseRoot(size_t &pos, const fileVector &file, ServerConfig &server) {
    std::string val = getDirectiveValue(pos, file, "root");
    if (val.empty())
		return -1;
    server.setRoot(val);
    return pos;
}

static int parseServerName(size_t &pos, const fileVector &file, ServerConfig &server) {
    pos++;
    if (pos >= file.size() || file[pos] == ";") {
        print_msg("server_name: missing value", ERR);
        return -1;
    }
    while (pos < file.size() && file[pos] != ";") {
        server.addServerName(file[pos]);
        pos++;
    }
    if (pos >= file.size() || file[pos] != ";") {
        print_msg("server_name: missing semicolon", ERR);
        return -1;
    }
    return pos;
}

static int parseErrorPage(size_t &pos, const fileVector &file, ServerConfig &server) {
    pos++;
    if (pos + 1 >= file.size() || file[pos] == ";") {
        print_msg("error_page: invalid format", ERR);
        return -1;
    }
    int code = std::atoi(file[pos].c_str());
    std::string path = file[pos + 1];
    server.addErrorPage(code, path);
    pos += 2;
    if (pos >= file.size() || file[pos] != ";") {
        print_msg("error_page: missing semicolon", ERR);
        return -1;
    }
    return pos;
}

static int redirectDictionary(const std::string &word)
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

static bool redirectServer(size_t &pos, const fileVector &file,
		ServerConfig &server, int serv)
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
			Location loc;
			ret = loc.parseLocation(pos, file, server, loc);	break;
	}
	return (ret);
}	

bool ServerConfig::parseServer(size_t &pos, const fileVector &file,
		ServerConfig &server)
{
    bool    status;
    int     servEnum;

    while (pos < file.size() && file[pos] != "}") {
        servEnum = redirectDictionary(file[pos]);
        if (servEnum == UNEXPECTED) {
            print_msg("Unexpected word in server: " + file[pos], ERR);
            return false;
        }
        status = redirectServer(pos, file, server, servEnum);
        if (!status) 
            return false;
        pos++;
    }
    this->_server_names.push_back(server.getHost());
    
    print_msg("Server parsed successfully :)", DEBUG);
    return true;
}

/*	costructor
 * */ 
ServerConfig::ServerConfig() : _port(0), _max_body_size(DEFAULT_SIZE), _host("") {}

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
		this->_index		  = obj._index;
		this->_max_body_size  = obj._max_body_size;
		this->_server_names   = obj._server_names;
		this->_error_pages	  = obj._error_pages;
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

const std::string& ServerConfig::getIndex() const {
	return _index;
}

int ServerConfig::getPort() const {
	return _port;
}

size_t ServerConfig::getMaxBodySize() const {
	return _max_body_size;
}

const std::string& ServerConfig::getHost() const {
	return _host; 
}

const std::vector<std::string>& ServerConfig::getServerNames() const {
	return _server_names;
}

const std::map<int, std::string>& ServerConfig::getErrorPages() const {
	return _error_pages;
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

void ServerConfig::setIndex(const std::string& index) {
	_index = index;
}

void ServerConfig::setHost(const std::string& host) {
	_host = host;
}

void ServerConfig::setMaxBodySize(size_t size) {
	_max_body_size = size;
}

void ServerConfig::addServerName(const std::string& name) {
	_server_names.push_back(name);
}

void ServerConfig::addErrorPage(int code, const std::string& path) {
	_error_pages[code] = path;
}

void ServerConfig::addLocation(const Location& location) {
	_locations.push_back(location);
}

void ServerConfig::setRoot(const std::string& root) {
	_root = root;
}
