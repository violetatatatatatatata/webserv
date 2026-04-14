/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:58:59 by avelandr          #+#    #+#             */
/*   Updated: 2026/04/14 12:47:53 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include <Webserv.hpp>
# include <exception>
# include <string>
# include <sstream>

class HttpException : public std::exception {
    private:
        int         _statusCode;
        std::string _message;

    public:
        HttpException(int code, const std::string& msg);
        virtual ~HttpException() throw();

        virtual const char* what() const throw();
        
        int		getStatusCode() const;
};

/* client_error:
 * - Error 400: Bad Request - The server cannot process the request due to invalid syntax.
 * - Error 403: Forbidden - The server understands the request but refuses to authorize it.
 * - Error 404: Not Found - The server cannot find the requested resource.
 * - Error 405: Method Not Allowed - The request method is known by the server but has been disabled.
 * - Error 413: Payload Too Large - The request entity is larger than limits defined by server.
 * - Error 414: URI Too Long - The URI requested by the client is longer than the server is willing to interpret.
 */
class BadRequestException : public HttpException {
    public: BadRequestException() : HttpException(400, "Bad Request") {}
};

class ForbiddenException : public HttpException {
    public: ForbiddenException() : HttpException(403, "Forbidden") {}
};

class NotFoundException : public HttpException {
    public: NotFoundException() : HttpException(404, "Not Found") {}
};

class MethodNotAllowedException : public HttpException {
    public: MethodNotAllowedException() : HttpException(405, "Method Not Allowed") {}
};

class PayloadTooLargeException : public HttpException {
    public: PayloadTooLargeException() : HttpException(413, "Payload Too Large") {}
};

class UriTooLongException : public HttpException {
    public: UriTooLongException() : HttpException(414, "URI Too Long") {}
};

/* server_error:
 * - Error 500: Internal Server Error - The server encountered an unexpected condition.
 * - Error 501: Not Implemented - The server does not support the functionality required to fulfill the request.
 * - Error 504: Gateway Timeout - The server, while acting as a gateway, did not receive a timely response from the upstream (CGI).
 * - Error 505: HTTP Version Not Supported - The server does not support the HTTP protocol version used in the request.
 */
class InternalServerErrorException : public HttpException {
    public: InternalServerErrorException() : HttpException(500, "Internal Server Error") {}
};

class NotImplementedException : public HttpException {
    public: NotImplementedException() : HttpException(501, "Not Implemented") {}
};

class GatewayTimeoutException : public HttpException {
    public: GatewayTimeoutException() : HttpException(504, "Gateway Timeout") {}
};

class VersionNotSupportedException : public HttpException {
    public: VersionNotSupportedException() : HttpException(505, "HTTP Version Not Supported") {}
};

#endif
