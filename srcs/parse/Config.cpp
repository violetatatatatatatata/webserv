/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:17:35 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/30 16:49:35 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

namespace Config {

/*	FROM SERVER
 * */

// getters

int Server::getPort() {
	return _port;
}

size_t Server::getMaxBodySize() {
	return _max_body_size;
}

std::string& Server::getHost() {
	return _host;
}

std::vector<std::string>& Server::getServerNames() {
	return _server_names;
}

std::map<int, std::string>& Server::getErrorPages() {
	return _error_pages;
}

std::vector<Location>& Server::getLocations() {
	return _locations;
}

// server

void Server::setPort(int port) {
	_port = port;
}

void Server::setHost(std::string& host) {
	_host = host;
}

void Server::setMaxBodySize(size_t size) {
	_max_body_size = size;
}

void Server::addServerName(std::string& name) {
	_server_names.push_back(name);
}

void Server::addErrorPage(int code, std::string& path) {
	_error_pages[code] = path;
}

void Server::addLocation(Location& location) {
	_locations.push_back(location);
}

/*	FROM LOCATION
 * */

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

}
