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

/*	INCLUDES */
# include <iostream>
# include <iomanip>
# include <sstream>
# include <fstream>
# include <string>
# include <limits>
# include <cstdio>
# include <cstring>

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
# include <csignal>
# include <string.h>
# include <cstdlib>
# include <fcntl.h>
# include <time.h>
# include <limits.h>
# include <errno.h>
# include <dirent.h>
# include <ctime>
# include <poll.h>

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

/* GLOBAL DEFINITIONS */
# define CYAN "\x1b[36m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define LBLUE "\x1B[38;2;53;149;240m"
# define BLUE "\x1b[34m"
# define ROSE "\x1B[38;2;255;151;203m"
# define MAGENTA "\x1b[35m"
# define BOLD "\033[1;37m"
# define RESET "\033[0m"

# define DEFAULT_SIZE 1048576

typedef std::vector<std::string> fileVector;

enum e_level {
	INFO,
	SUCCESS,
	ERR,
	FATAL,
	START,
	CONN,
	DISC,
	DEBUG,
	REQ,
	RES,
	WARN,
	CGI_LOG
};

int print_msg(std::string msg, int status);


#endif
