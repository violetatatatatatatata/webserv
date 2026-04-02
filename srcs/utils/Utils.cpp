/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:54:00 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/02 17:34:38 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Webserv.hpp>

static std::string build_msg(int status) {
    struct tm   tstruct;
    time_t      now;
    char        buf[100];

    now = time(0);
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%X] ", &tstruct);
    
	std::string msg(buf);

    switch (status) {
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
    return msg;
}

int	print_msg(std::string msg, int status)
{
	int	ret = ((status != ERR) || (status != FATAL));
    std::string color = RESET;

    switch(status) {
        case ERR: case FATAL:
			color = RED;	break;
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
		default:
			color = BOLD;
    }
    std::cout << color << build_msg(status) << msg << RESET << std::endl;
	return (ret);
}

std::string getDirectiveValue(size_t &pos, const fileVector &file, const std::string &directiveName) {
	int	exit_status;

	if (pos >= file.size() || file[pos] == ";")
	{
        exit_status = print_msg(directiveName + ": missing value", ERR);
		return NULL;
	}
    std::string value = file[pos];
    if (value.empty()) {
		exit_status = print_msg(directiveName + " value cannot be empty!", ERR);
		return NULL;
	}
	pos++;
    if (pos >= file.size() || file[pos] != ";") {
        exit_status = print_msg(directiveName + ": missing semicolon ';'", ERR);
		return NULL;
	}
	(void)exit_status;

	return value;
}

