#pragma once

#include <Response.hpp>
#include <ServerParser.hpp>
#include <LocationParser.hpp>
#include <string>
#include <map>

class Request
{
    public:

        Request();
        ~Request();

        // Methods
        int parse(const std::string& raw);
        bool isChunked() const;

        //void print();

        // Getters
        int                 getclientFd() const;
        bool                getIsChunked() const;
        const std::string&  getMethod() const;
        const std::string&  getVersion() const;
        const std::string&  getURI() const;
        const std::string&  getBody() const;
        std::string  getHeader(const std::string& header) const;

        // Setters
        void setclientFd(int clientFd);
        void setPort(int port);
        void setMethod(const std::string& method);
        void setVersion(std::string& httpVersion);
        void setURI(const std::string& URI);
        void setBody(std::string& body);
        void setHeader(const std::string& header, const std::string& value);

    private:

        Request(const Request& other);
        Request& operator=(const Request& other);

        int                                 _clientFd;
        bool                                _isChunked;
        std::string                         _method;
        std::string                         _httpVersion;
        std::string                         _URI;
        std::string                         _body;
        std::map<std::string, std::string>  _headers;
};
