#pragma once

#include <Request.hpp>
#include <ServerParser.hpp>
#include <sstream>
#include <string>
#include <map>
#include <iostream>

class Response
{
    public:

        Response();
        Response(const Response& other);
        ~Response();
        Response& operator=(const Response& other);

        // Methods
        void fillHeaders();
        std::string findMIME(const std::string& path) const;
        std::string buildResponse();

        // Setters
        void setSocketFd(int socket_Fd);
        void setVersion(const std::string& version);
        void setHeader(const std::string& header, const std::string& value);
        void setResponseData(int error, const std::string& reasonPhrase, const std::string& body);
        void setPipeFd(int pipeFds);

        // Getters
        int getPipeFd() const;

    private:

        int _error_code;
        int _socket_Fd;
        int _pipeFd;
        std::string _version;
        std::string _body;
        std::string _reasonPhrase;
        std::map<std::string, std::string>  _headers;
};
