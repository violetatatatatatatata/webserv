/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 ServerParser.cpp									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: avelandr <avelandr@student.42barcelon		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/03/31 14:40:51 by avelandr		   #+#	  #+#			  */
/*	 Updated: 2026/04/01 00:22:37 by avelandr		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include <ServerParser.hpp>

class LocationBlockAction : public IAction {
	public:
		virtual IAction* clone() const {
			return new LocationBlockAction();
		}
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) {
			ServerParser* server = static_cast<ServerParser*>(obj);

			LocationParser loc;
			if (!loc.parseLocation(pos, file, *server))
				return false;

			return true;
		}
};

ServerParser::ServerParser() : Parser(), _port(0), _max_body_size(DEFAULT_SIZE), _host("") {
	initMap();
}

ServerParser::ServerParser(const ServerParser &obj) : Parser(obj) {
	*this = obj;
}

ServerParser &ServerParser::operator=(const ServerParser &obj) {
	if (this != &obj) {
		Parser::operator=(obj);
		this->_port			  = obj._port;
		this->_host			  = obj._host;
		this->_index		  = obj._index;
		this->_max_body_size  = obj._max_body_size;
		this->_server_names   = obj._server_names;
		this->_locations	  = obj._locations;
		this->_root           = obj._root;
	}
	return *this;
}

ServerParser::~ServerParser() {}

void ServerParser::initMap()
{
	registerAction("listen", new SimpleAction<ServerParser, int>(&ServerParser::_port, "listen"));
	registerAction("host", new SimpleAction<ServerParser, std::string>(&ServerParser::_host, "host"));
	registerAction("root", new SimpleAction<ServerParser, std::string>(&ServerParser::_root, "root"));
	registerAction("index", new SimpleAction<ServerParser, std::string>(&ServerParser::_index, "index"));
	registerAction("client_max_body_size", new SimpleAction<ServerParser, size_t>(&ServerParser::_max_body_size, "client_max_body_size"));
	registerAction("server_name", new VectorAction<ServerParser, std::string>(&ServerParser::_server_names, "server_name"));
	registerAction("location", new LocationBlockAction());
}

bool ServerParser::parseServer(size_t &pos, const fileVector &file)
{
	if (!this->parseBlock(pos, file))
		return false;
	this->_server_names.push_back(this->getHost());
	if (this->_port > 65535) {
		print_msg("Invalid argument.", ERR);
		return false;
	}   
	if (this->_port > 0 && this->_port < 1024) {
		print_msg("Attempting to connect to a reserved port.", WARN);
	}   

	return true;
}

bool ServerParser::hasServerName(const std::string& server_name) const {
	for (size_t i = 0; i < _server_names.size(); i++) {
		if (_server_names[i] == server_name)
			return true;
	}
	return false;
}

void ServerParser::addLocation(const LocationParser& location) {
	_locations.push_back(location);
}

int ServerParser::getPort() const {
	return _port;
}

size_t ServerParser::getMaxBodySize() const {
	return _max_body_size;
}

const std::string& ServerParser::getHost() const {
	return _host;
}

const std::string& ServerParser::getRoot() const {
	return _root;
}

const std::string& ServerParser::getIndex() const {
	return _index;
}

const std::vector<std::string>& ServerParser::getServerNames() const {
	return _server_names;
}

const std::vector<LocationParser>& ServerParser::getLocations() const {
	return _locations;
}
