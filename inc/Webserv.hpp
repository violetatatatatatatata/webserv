/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:45:43 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/14 12:47:05 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*	INCLUDES
 * */
# include <iostream>
# include <iomanip>
# include <sstream>
# include <fstream>
# include <string>
# include <limits>
# include <cstdio>

// CPP containers
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>
# include <utility>


// C Includes
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <cstdlib>
# include <fcntl.h>
# include <time.h>
# include <limits.h>
# include <errno.h>
# include <dirent.h>
# include <ctime>

// C System
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>

// C Network
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <netinet/in.h>

/*	aqui se carga TODO el file como un vector de strings (**file)
 * */
typedef std::vector<std::string> fileVector;

// Implemented classes
#include <AutoIndexHandler.hpp>
#include <RedirectHandler.hpp>
#include <CGIHandler.hpp>
#include <HandlerFactory.hpp>
#include <HttpHandler.hpp>
#include <Request.hpp>
#include <Router.hpp>
#include <Response.hpp>
#include <Client.hpp>
#include <Cluster.hpp>
#include <Server.hpp>
#include <Config.hpp>
#include <Location.hpp>
#include <Reader.hpp>
#include <ServerConfig.hpp>
#include <Webserv.hpp>

/*	GLOBAL DEFINITIONS
 * */
# define CYAN "\x1b[36m"
# define RED "\033[31m"					// for failure
# define GREEN "\033[32m"				// for success and start
# define YELLOW "\033[33m"				
# define LBLUE "\x1B[38;2;53;149;240m"	// request
# define BLUE "\x1b[34m"				// response
# define ROSE "\x1B[38;2;255;151;203m"	// client connected
# define MAGENTA "\x1b[35m"				// client disconnected and cgi log
# define BOLD "\033[1;37m"				// for debugging porpuses
# define RESET "\033[0m"

//		1048576 bytes = 1MB por defecto
# define DEFAULT_SIZE 1048576

/* ENUM
 * */
enum e_level {
    INFO,		// general message
	SUCCESS,
	ERR,		// recoverable error
	FATAL,		// non recoverable error, exit
	START,		// for the exact moment when the server starts running
	CONN,		// client status connected
	DISC,		// client status disconnected
	DEBUG,		// messages that might be deleted before evaluation
	REQ,		// request
	RES,		// response
	WARN,		// recoverable error that can be fixed during execution
	CGI_LOG		// cgi stuff
};

enum e_module {
    PARSER,
	CORE,
	HTTP,
	CGI
};

/*	FUNCTION DEFINITIONS
 * */
int			print_msg(std::string msg, int status);
void		buildError(int code, ServerConfig config);
std::string	getDirectiveValue(size_t &pos, const fileVector &file, const std::string &directiveName);

#endif
