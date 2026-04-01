/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:42:54 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/01 02:17:57 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

static int parseRootLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "root");
    loc.setRoot(val);
    return pos;
}

static int parseIndexLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "index");
    loc.setIndex(val);
    return pos;
}

static int parseUploadStoreLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "upload_store");
    loc.setUploadStore(val);
    return pos;
}

static int parseAutoindexLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "autoindex");
    if (val == "on")
		loc.setAutoindex(true);
    else if (val == "off")
		loc.setAutoindex(false);
    else
		return (print_msg("autoindex: invalid value (must be 'on' or 'off')", ERR));
    return pos;
}

static int parseMethodsLoc(size_t pos, const fileVector &file, Location &loc) {
    if (pos >= file.size() || file[pos] == ";")
        return (print_msg("methods: missing values", ERR));
    while (pos < file.size() && file[pos] != ";") {
        loc.addMethod(file[pos]);
        pos++;
    }
    if (pos >= file.size() || file[pos] != ";")
        return (print_msg("methods: missing semicolon", ERR));
    return pos;
}

static int parseCgiInfoLoc(size_t pos, const fileVector &file, Location &loc) {
    if (pos + 1 >= file.size())
        return (print_msg("cgi_info: invalid format", ERR));
    std::string ext = file[pos];
    std::string bin = file[pos + 1];
    loc.addCgiInfo(ext, bin);
    pos += 2;
    if (pos >= file.size() || file[pos] != ";")
        return (print_msg("cgi_info: missing semicolon", ERR));
    return pos;
}

static int redirectDictionaryLoc(const std::string &word) {
    if (word == "root")             	return LOC_ROOT;
    if (word == "index")            	return LOC_INDEX;
    if (word == "autoindex")			return LOC_AUTOINDEX;
    if (word == "methods"
			|| word == "allow_methods") return LOC_METHODS;
    if (word == "upload_store")     	return LOC_UPLOAD_STORE;
    if (word == "cgi_info")         	return LOC_CGI_INFO;
    return LOC_UNEXPECTED;
}

int	parseLocation(size_t pos, fileVector file) {
	unsigned int	status;

	while (pos < file.size() && file[pos] != '}') {
		if (file[pos] == "location") {
			ConfigLocation loc;
            if (file[++i] != "{")
                return (print_msg("Expected '{' after location directive", ERR));
            if (!redirectLocation(++pos, file, loc))
                return (print_msg("Error in location configuration", ERR));
			pos++;
		}
		else
			return (print_msg("Missing location", ERR));
		this->_locations.push_back(location);
	}
	return (print_msg("Location parsed successfully :)", DEBUG));
}

Location::Location() 
    : _path(""), _autoindex(false), _index(""), _root(""), 
      _uploadStore(""), _cgiInfo(""), _redirect("") {}

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
