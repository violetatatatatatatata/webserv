#pragma once

#include <algorithm>
#include <string>
#include <map>

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
        const std::string&  getMethod() const;
        const std::string&  getVersion() const;
        const std::string&  getURI() const;
        const std::string&  getBody() const;
        std::string  getHeader(const std::string& header) const;
        
        // Setters
        void setSocketFd(int socketFd);
        void setPort(int port);
        void setMethod(const std::string& method);
        void setVersion(std::string& httpVersion);
        void setURI(std::string& URI); 
        void setBody(std::string& body);
        void setHeader(const std::string& header, std::string& value);

    private:

        int                                 _port;
        int                                 _socketFd;
        std::string                         _method;
        std::string                         _httpVersion;
        std::string                         _URI;
        std::string                         _body;
        std::map<std::string, std::string>  _headers;
};