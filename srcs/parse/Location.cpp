/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:42:54 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/07 15:45:20 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

static int parseReturnLoc(size_t &pos, const fileVector &file, Location &loc) {
    pos++;
    if (pos >= file.size() || file[pos] == ";") {
        print_msg("return: missing value", ERR);
        return -1;
    }
    std::string redirect_val = "";
    while (pos < file.size() && file[pos] != ";") {
        if (!redirect_val.empty())
            redirect_val += " ";
        redirect_val += file[pos];
        pos++;
    }
    if (pos >= file.size() || file[pos] != ";") {
        print_msg("return: missing semicolon", ERR);
        return -1;
    }
    loc.setRedirect(redirect_val);
    return pos;
}

static int parseRootLoc(size_t &pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "root");
    if (val.empty())
		return -1;
    loc.setRoot(val);
    return pos;
}

static int parseIndexLoc(size_t &pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "index");
    if (val.empty())
		return -1;
    loc.setIndex(val);
    return pos;
}

static int parseUploadStoreLoc(size_t &pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "upload_store");
    if (val.empty())
		return -1;
    loc.setUploadStore(val);
    return pos;
}

static int parseAutoindexLoc(size_t &pos, const fileVector &file, Location &loc) {
    std::string val = getDirectiveValue(pos, file, "autoindex");
    if (val.empty())
		return -1;
    if (val == "on") loc.setAutoindex(true);
    else if (val == "off") loc.setAutoindex(false);
    else {
        print_msg("autoindex: invalid value (must be 'on' or 'off')", ERR);
        return -1;
    }
    return pos;
}

static long parseMethodsLoc(size_t &pos, const fileVector &file, Location &loc) {
    pos++;
    if (pos >= file.size() || file[pos] == ";") {
        print_msg("methods: missing values", ERR);
        return -1;
    }
    while (pos < file.size() && file[pos] != ";") {
        loc.addMethod(file[pos]);
        pos++;
    }
    if (pos >= file.size() || file[pos] != ";") {
        print_msg("methods: missing semicolon", ERR);
        return -1;
    }
    return pos;
}

static int parseCgiLoc(size_t &pos, const fileVector &file, Location &loc) {
    pos++;
    if (pos + 1 >= file.size() || file[pos] == ";") {
        print_msg("cgi_info: invalid format", ERR);
        return -1;
    }
    std::string ext = file[pos];
    std::string bin = file[pos + 1];
    loc.addCgiInfo(ext, bin);
    pos += 2;
    if (pos >= file.size() || file[pos] != ";") {
        print_msg("cgi_info: missing semicolon", ERR);
        return -1;
    }
    return pos;
}

static int redirectLocation(size_t &pos, const fileVector &file, Location &loc, int locEnum)
{
    int ret = -1;
    switch (locEnum) {
        case LOC_ROOT:         
			ret = parseRootLoc(pos, file, loc);			break;
        case LOC_INDEX:         
			ret = parseIndexLoc(pos, file, loc); 		break;
        case LOC_AUTOINDEX:     
			ret = parseAutoindexLoc(pos, file, loc); 	break;
        case LOC_METHODS:       
			ret = parseMethodsLoc(pos, file, loc); 		break;
        case LOC_UPLOAD_STORE:  
			ret = parseUploadStoreLoc(pos, file, loc); 	break;
        case LOC_CGI_INFO:      
			ret = parseCgiLoc(pos, file, loc); 			break;
		case LOC_RETURN:
            ret = parseReturnLoc(pos, file, loc);       break;
    }
    return ret;
}

static int redirectDictionary(const std::string &word) {
    if (word == "root")                 return LOC_ROOT;
    if (word == "index")                return LOC_INDEX;
    if (word == "autoindex")            return LOC_AUTOINDEX;
    if (word == "methods"
            || word == "allow_methods") return LOC_METHODS;
    if (word == "upload_store")         return LOC_UPLOAD_STORE;
    if (word == "cgi_info")             return LOC_CGI_INFO;
	if (word == "return")               return LOC_RETURN;
    return LOC_UNEXPECTED;
}

void Location::resolveLocType()
{
    if (!_redirect.empty())
    {
        _locType = REDIRECT_TYPE;
    }
    else if (!_cgi_info.empty())
    {
        _locType = CGI_TYPE;
    }
    else if (_autoindex)
    {
        _locType = AUTOINDEX_TYPE;
    }
    else
    {
        _locType = STATIC_TYPE;
    }
}

bool Location::parseLocation(size_t &pos, const fileVector &file, 
		ServerConfig &s, Location &loc)
{
    int    status;
    int     locEnum;

    if (pos + 2 >= file.size() || file[pos + 2] != "{") {
        print_msg("Expected '{' after location path", ERR);
        return -1;
    }
    this->setPath(file[pos + 1]);
    pos += 3;
    while (pos < file.size() && file[pos] != "}") {
        locEnum = redirectDictionary(file[pos]);
        if (locEnum == LOC_UNEXPECTED) {
            print_msg("Unexpected word in location: " + file[pos], ERR);
            return -1;
        }
        status = redirectLocation(pos, file, loc, locEnum);
        if (status < 0)
            return -1;
        pos = static_cast<size_t>(status);
        pos++;
    }
    if (pos >= file.size() || file[pos] != "}") {
        print_msg("Location block not closed properly", FATAL);
        return -1;
    }
    resolveLocType();
    s.addLocation(*this);
    
	return pos;
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
        this->_locType      = obj._locType;
    }
    return *this;
}

Location::~Location() {}

// getters

bool Location::getAutoindex() const {
    return _autoindex;
}

e_locType Location::getLocType() const {
    return _locType;
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
