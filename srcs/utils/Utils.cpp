/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:54:00 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/01 01:05:48 by avelandr         ###   ########.fr       */
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
    return msg;
}

int	print_msg(int level, std::string msg)
{
	int			ret = (level != ERR || level != FATAL);
    std::string color = RESET;

    switch(level) {
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
		default
			color = BOLD;
    }
    std::cout << color << build_msg(level) << msg << RESET << std::endl;
	return (ret);
}

void std::string getDirectiveValue(int &pos, const fileVector &file, const std::string &directiveName) {
    if (pos >= file.size() || file[pos] == ";")
        return (print_msg(directiveName + ": missing value", ERR));
    std::string value = file[pos];
    if (value.empty())
		return (print_msg(directiveName + " value cannot be empty!", ERR));
    pos++;
    if (pos >= file.size() || file[pos] != ";")
        return (print_msg(directiveName + ": missing semicolon ';'", ERR));
    return value;
}

