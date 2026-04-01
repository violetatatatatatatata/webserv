#pragma once

#include "../Webserv.hpp"

enum e_method
{
    GET,
    POST,
    DELETE
};

class Request
{
    public:

        Request();
        Request(const Request& other);
        ~Request();
        Request& operator=(const Request& other);

        // Methods


        // Getters
        e_method        getMethod();
        std::string&    getVersion();
        std::string&    getURI();
        std::string&    getBody();
        
        // Setters
        void setMethod(e_method method);
        void setVersion(std::string version);
        void setURI(std::string path); 
        void setBody(std::string body);

    private:

        e_method                            m_method;
        std::string                         m_httpVersion;
        std::string                         m_URI;
        std::string                         m_body;
        std::map<std::string, std::string>  m_headers;
};