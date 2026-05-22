/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:05:36 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/02 16:31:52 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONPARSER_HPP
# define LOCATIONPARSER_HPP

# include <Parser.hpp>

class ServerParser;

/*
 *	_autoindex: indicates whether automatic directory listing is on or off
 *	_index: default file to be served in this route
 *	_path: the route or URI defining the scope of this block (e.g. /cgi-bin)
 *	_root: physical root directory assigned to this specific route
 *	_upload_store: physical directory where client-uploaded files will be saved
 *	_redirect: URL or path to which traffic will be redirected (return directive)
 *	_methods: list of allowed HTTP methods (GET, POST, DELETE)
 *	_cgi_info: map linking file extensions with their respective binaries (legacy)
 *	_cgi_ext: vector containing file extensions considered as CGI scripts (e.g. .py, .php)
 *	_cgi_path: vector containing absolute paths to the CGI executables
 *
 *	LocationParser: default constructor
 *	LocationParser (copy): copy constructor
 *	~LocationParser: class destructor
 *	operator=: assignment operator overload
 *
 *	initMap: instantiates and registers parsing actions for allowed directives in the location block
 *	parseLocation: processes the route, extracts block tokens, and links the location to its server
 *	getAutoindex: returns true if directory listing is enabled
 *	getPath: returns the URI associated with the location
 *	getRoot: returns the root directory of the route
 *	getIndex: returns the configured index file
 *	getUploadStore: returns the path for storing uploads
 *	getRedirect: returns the HTTP redirection configuration
 *	getMethods: returns the authorized HTTP methods
 *	getCgiInfo: returns the complete map of legacy CGI configurations
 *	getCgiInfo (ext): searches and returns the executable associated with a specific extension
 *	getCgiExt: returns the list of configured CGI extensions
 *	getCgiPath: returns the list of configured CGI executables
 * */
class LocationParser : public Parser {
	private:
		bool								_autoindex;
		std::string							_index;
		std::string							_path;
		std::string							_root;
		std::string							_upload_store;
		std::string							_redirect;
		std::vector<std::string>			_methods;
		std::map<std::string, std::string>	_cgi_info;
		std::vector<std::string>			_cgi_ext;
		std::vector<std::string>			_cgi_path;

	public:
		LocationParser();
		LocationParser(const LocationParser &obj);
		LocationParser &operator=(const LocationParser &obj);
		virtual ~LocationParser();

		virtual void initMap();

		bool parseLocation(size_t &pos, const fileVector &file, ServerParser &s);

		bool										getAutoindex() const;
		const std::string&							getPath() const;
		const std::string&							getRoot() const;
		const std::string&							getIndex() const;
		const std::string& 							getUploadStore() const;
		const std::string&  						getRedirect() const;
		const std::vector<std::string>& 			getMethods() const;
		const std::map<std::string, std::string>&	getCgiInfo() const;
		std::string									getCgiInfo(const std::string& ext) const;
		const std::vector<std::string>&             getCgiExt() const;
		const std::vector<std::string>& 			getCgiPath() const;
};

#endif
