#pragma once

#include <algorithm>
#include <string>
#include <map>

enum e_method
{
    GET,
    POST,
    DELETE
};

class Request
{
    public:

        Request(int i);
        Request();
        Request(const Request& other);
        ~Request();
        Request& operator=(const Request& other);

        // Methods

        // Getters
        int                 getSocketFd() const;
        int                 getPort() const;
        e_method            getMethod() const;
        const std::string&  getVersion() const;
        const std::string&  getURI() const;
        const std::string&  getBody() const;
        std::string  getHeader(const std::string& header) const;
        
        // Setters
        void setSocketFd(int socketFd);
        void setPort(int port);
        void setMethod(e_method method);
        void setVersion(std::string& httpVersion);
        void setURI(std::string& URI); 
        void setBody(std::string& body);
        void setHeader(const std::string& header, std::string& value);

    private:

        int                                 _port;
        int                                 _socketFd;
        e_method                            _method;
        std::string                         _httpVersion;
        std::string                         _URI;
        std::string                         _body;
        std::map<std::string, std::string>  _headers;
};