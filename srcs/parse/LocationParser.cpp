/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:42:54 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/07 15:45:20 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <LocationParser.hpp>
#include <ServerParser.hpp>

LocationParser::LocationParser()
	: Parser(), _autoindex(false), _index(""), _path(""), _root(""),
	_upload_store(""), _redirect("")
{
	initMap();
}

LocationParser::LocationParser(const LocationParser &obj) : Parser(obj) {
	*this = obj;
}

LocationParser &LocationParser::operator=(const LocationParser &obj) {
	if (this != &obj) {
		Parser::operator=(obj);
		this->_path         = obj._path;
		this->_root         = obj._root;
		this->_index        = obj._index;
		this->_autoindex    = obj._autoindex;
		this->_methods      = obj._methods;
		this->_upload_store = obj._upload_store;
		this->_cgi_info     = obj._cgi_info;
		this->_redirect     = obj._redirect;
		this->_cgi_ext      = obj._cgi_ext;
		this->_cgi_path     = obj._cgi_path;
	}
	return *this;
}

LocationParser::~LocationParser() {}

void LocationParser::initMap()
{
	registerAction("root", new SimpleAction<LocationParser, std::string>(&LocationParser::_root, "root"));
	registerAction("index", new SimpleAction<LocationParser, std::string>(&LocationParser::_index, "index"));
	registerAction("upload_store", new SimpleAction<LocationParser, std::string>(&LocationParser::_upload_store, "upload_store"));
	registerAction("autoindex", new BoolAction<LocationParser>(&LocationParser::_autoindex, "autoindex"));
	registerAction("methods", new VectorAction<LocationParser, std::string>(&LocationParser::_methods, "methods"));
	registerAction("allow_methods", new VectorAction<LocationParser, std::string>(&LocationParser::_methods, "allow_methods"));
	registerAction("cgi_info", new MapAction<LocationParser, std::string, std::string>(&LocationParser::_cgi_info, "cgi_info"));
	registerAction("return", new StringConcatAction<LocationParser>(&LocationParser::_redirect, "return"));
	registerAction("cgi_ext", new VectorAction<LocationParser, std::string>(&LocationParser::_cgi_ext, "cgi_ext"));
	registerAction("cgi_path", new VectorAction<LocationParser, std::string>(&LocationParser::_cgi_path, "cgi_path"));
}

std::string normalizeLocation(std::string path)
{
    std::string result;
    bool prev = false;

    for (size_t i = 0; i < path.size(); i++)
    {
        if (path[i] == '/')
        {
            if (!prev)
                result += '/';
            prev = true;
        }
        else
        {
            result += path[i];
            prev = false;
        }
    }

    if (result.size() > 1 && result[result.size() - 1] == '/')
        result.erase(result.size() - 1);

    return result;
}

bool LocationParser::parseLocation(size_t &pos, const fileVector &file,
		ServerParser &s)
{
	if (pos + 2 >= file.size() || file[pos + 2] != "{") {
		print_msg("Expected '{' after location path", ERR);
		return false;
	}
	this->_path = normalizeLocation(file[pos + 1]);
	//this->_path = file[pos + 1];
	pos += 3;
	if (!this->parseBlock(pos, file))
		return false;
	s.addLocation(*this);

	return true;
}

bool LocationParser::getAutoindex() const {
    return _autoindex;
}
const std::string& LocationParser::getPath() const {
    return _path;
}
const std::string& LocationParser::getRoot() const {
    return _root;
}
const std::string& LocationParser::getIndex() const {
    return _index;
}
const std::string& LocationParser::getUploadStore() const {
    return _upload_store;
}
const std::vector<std::string>& LocationParser::getMethods() const {
    return _methods;
}
const std::string& LocationParser::getRedirect() const {
    return _redirect;
}
const std::map<std::string, std::string>& LocationParser::getCgiInfo() const {
    return _cgi_info;
}

std::string LocationParser::getCgiInfo(const std::string& ext) const {
	std::map<std::string, std::string>::const_iterator it = _cgi_info.find(ext);
	if (it != _cgi_info.end())
		return it->second;
	else
		return std::string();
}

const std::vector<std::string>& LocationParser::getCgiExt() const {
    return _cgi_ext;
}

const std::vector<std::string>& LocationParser::getCgiPath() const {
    return _cgi_path;
}
