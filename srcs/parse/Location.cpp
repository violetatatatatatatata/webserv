/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:42:54 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/02 16:08:56 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

static size_t parseRootLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "root");
    loc.setRoot(val);
    return pos;
}

static size_t parseIndexLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "index");
    loc.setIndex(val);
    return pos;
}

static size_t parseUploadStoreLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "upload_store");
    loc.setUploadStore(val);
    return pos;
}

static size_t parseAutoindexLoc(size_t pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "autoindex");
    if (val == "on")
		loc.setAutoindex(true);
    else if (val == "off")
		loc.setAutoindex(false);
    else
		return (print_msg("autoindex: invalid value (must be 'on' or 'off')", ERR));
    return pos;
}

static size_t parseMethodsLoc(size_t pos, const fileVector &file, Location &loc) {
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

static size_t parseCgiLoc(size_t pos, const fileVector &file, Location &loc) {
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

static int redirectDictionary(const std::string &word) {
    if (word == "root")             	return LOC_ROOT;
    if (word == "index")            	return LOC_INDEX;
    if (word == "autoindex")			return LOC_AUTOINDEX;
    if (word == "methods"
			|| word == "allow_methods") return LOC_METHODS;
    if (word == "upload_store")     	return LOC_UPLOAD_STORE;
    if (word == "cgi_info")         	return LOC_CGI_INFO;
    return LOC_UNEXPECTED;
}

static int redirectLocation(size_t pos, fileVector file, Location loc, int locEnum)
{
	int	ret;

	pos++;
	switch (locEnum) {
		case LOC_ROOT:
			ret = parseRootLoc(pos, file, loc);		break;
		case LOC_INDEX:
			ret = parseIndexLoc(pos, file, loc);			break;
		case LOC_AUTOINDEX:
			ret = parseAutoindexLoc(pos, file, loc);	break;
		case LOC_METHODS:
			ret = parseMethodsLoc(pos, file, loc);	break;
		case LOC_UPLOAD_STORE:
			ret = parseUploadStoreLoc(pos, file, loc);		break;
		case LOC_CGI_INFO:
			ret = parseCgiLoc(pos, file, loc);			break;
	}
	return (ret);
}

int Location::parseLocation(size_t pos, fileVector file, ServerConfig &s)
{
	int	status;
	int				locEnum;
	Location		loc;

	while (pos < file.size() && file[pos] != "}") {
		if (file[++pos] != "{")
        	return (print_msg("Expected '{' after location directive", ERR));
		locEnum = redirectDictionary(file[pos]);
		if (locEnum == LOC_UNEXPECTED)
			return (print_msg("Unexpected word in location configuration!", ERR));
		status = redirectLocation(pos, file, loc, locEnum);
		if (status == ERR)
			return (print_msg("[LOC] Missing configuration in config file", WARN));
		else if (status == FATAL)
			return (print_msg("[LOC] Syntax failed!", FATAL));
		pos++;
	}
	s.addLocation(loc);

	return (print_msg("Location parsed successfully :)", DEBUG));
}

Location::Location()
    : _autoindex(false), _index(""), _path(""), _root(""),
	_upload_store(""), _redirect("") {}

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
        this->_redirect     = obj._redirect;    
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

const std::string& Location::getRedirect() const {
    return _redirect;
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

void Location::setRedirect(const std::string& redirect) {
    _redirect = redirect;
}

void Location::addCgiInfo(const std::string& ext, const std::string& bin) {
    _cgi_info[ext] = bin;
}
