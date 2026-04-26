/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 12:00:20 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/14 12:47:39 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Error.hpp"

HttpException::HttpException(int code, const std::string& msg) 
    : _statusCode(code), _message(msg) {}

HttpException::~HttpException() throw() {}

const char* HttpException::what() const throw() {
    return _message.c_str();
}

int HttpException::getStatusCode() const {
    return _statusCode;
}

/*	mira si el usuario definió una página de error en el .conf y si no, usar las propias
 * */
std::string buildError(int code, const ServerConfig& config)
{
    std::string html_path;
    if (config.getErrorPages().count(code) > 0)
        html_path = config.getErrorPages().at(code);
    else
    {
        switch (code) {
            case 400: html_path = "www/client_error/400bad_request.html";
					  break;
            case 403: html_path = "www/client_error/403permission_denied.html";
					  break;
            case 404: html_path = "www/client_error/404page_not_found.html";
					  break;
            case 405: html_path = "www/client_error/405method_not_allowed.html";
					  break;
            case 413: html_path = "www/client_error/413payload_too_large.html";
					  break;
            case 414: html_path = "www/client_error/414uri_too_long.html";
					  break;
            case 501: html_path = "www/internal_error/501not_implemented.html";
					  break;
            case 504: html_path = "www/internal_error/504gateway_timeout.html";
					  break;
            case 505: html_path = "www/internal_error/505version_not_supported.html";
					  break;
            default:  html_path = "www/internal_error/500internal_error.html";
					  break;
        }
    }

    return html_path;
}
