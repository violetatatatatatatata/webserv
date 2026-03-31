/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:42:54 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 18:43:25 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

namespace Config {

static int redirectLocation(loc) {
	int	ret;

	switch (loc) {
		case PATH:
			ret = parsePath()
		case METHODS:

		case ROOT:

		case INDEX:

		case AUTOINDEX:

		case UPLOAD_STORE:

		case CGI:

		case RETURN:
	}
}

int	parseLocation(int pos, fileVector file) {
	unsigned int	status;

	while (pos++ < file.size() && file[pos] != '}') {
		if (file[pos] == "location") {
			ConfigLocation loc;
            if (file[++i] != "{")
                return (print_msg("Expected '{' after location directive", ERR));
            if (!redirectLocation(++pos, file, loc))
                return (print_msg("Error in location configuration", ERR));
		}
		else
			return (print_msg("Missing location", ERR));
		this->_locations.push_back(location);
	}
}

Location::Location() : _path(""), _root(""), _index(""), _autoindex(false),
	_upload_store("") {}

Location::Location(const Location &obj) {
    *this = obj;
}

Location &Location::operator=(const Location &obj) {
    if (this != &obj) {
        this->_path         = obj._path;
        this->_root         = obj._root;
        this->_index        = obj._index;
        this->_autoindex    = obj._autoindex;
        this->_methods      = obj._methods;     
        this->_upload_store = obj._upload_store;
        this->_cgi_info     = obj._cgi_info;    
    }
    return *this;
}

Location::~Location() {}

// getters

bool Location::getAutoindex() const {
    return _autoindex;
}

const std::string& Location::getPath() const {
    return _path;
}

const std::string& Location::getRoot() const {
    return _root;
}

const std::string& Location::getIndex() const {
    return _index;
}

const std::string& Location::getUploadStore() const {
    return _upload_store;
}

const std::vector<std::string>& Location::getMethods() const {
    return _methods;
}

const std::map<std::string, std::string>& Location::getCgiInfo() const {
    return _cgi_info;
}

// setters

void Location::setPath(const std::string& path) {
    _path = path;
}

void Location::setRoot(const std::string& root) {
    _root = root;
}

void Location::setIndex(const std::string& index) {
    _index = index;
}

void Location::setAutoindex(bool state) {
    _autoindex = state;
}

void Location::setUploadStore(const std::string& path) {
    _upload_store = path;
}

void Location::addMethod(const std::string& method) {
    _methods.push_back(method);
}

void Location::addCgiInfo(const std::string& ext, const std::string& bin) {
    _cgi_info[ext] = bin;
}

