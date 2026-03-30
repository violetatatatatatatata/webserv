/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:54:00 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/30 16:02:47 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

static std::string build_msg(int level, int mod) {
    struct tm   tstruct;
    time_t      now;
    char        buf[100];

    now = time(0);
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%X] ", &tstruct);
    
	std::string msg(buf);

    switch (level) {
        case INFO:		msg += "[INFO]\t";		break;
        case SUCCESS:	msg += "[SUCCESS]\t";	break;
        case ERR:		msg += "[ERROR]\t";		break;
        case FATAL:		msg += "[FATAL]\t"; 	break;
        case START:		msg += "[START]\t"; 	break;
        case CONN: 		msg += "[CONN]\t"; 		break;
        case DISC: 		msg += "[DISC]\t"; 		break;
        case DEBUG:		msg += "[DEBUG]\t"; 	break;
        case REQ:		msg += "[REQ]\t"; 		break;
        case RES:		msg += "[RES]\t"; 		break;
        case WARN:		msg += "[WARN]\t";		break;
        case CGI_LOG:	msg += "[CGI]\t"; 		break;
    }
    switch (mod) {
        case PARSER:	msg += "[Parser]\t";	break;
        case CORE:		msg += "[Core]\t"; 		break;
        case HTTP:		msg += "[HTTP]\t"; 		break;
        case CGI:		msg += "[Runner]\t";	break;
    }
    return msg;
}

void print_msg(int level, int mod, std::string msg) {
    std::string color = RESET;
    switch(level) {
        case ERR: case FATAL:
			color = RED;		break;
        case SUCCESS: case START:
			color = GREEN;		break;
        case WARN:
			color = YELLOW;		break;
		case CONN: 
			color = ROSE;		break;
		case DISC:
			color = MAGENTA;	break;
        case REQ:
			color = LBLUE;		break;
		case RES:
			color = BLUE;		break;
        case CGI_LOG:
			color = MAGENTA;	break;
        case INFO:
			color = CYAN;		break;
		default
			color = BOLD;
    }
    std::cout << color << build_msg(level, mod) << msg << RESET << std::endl;
}
