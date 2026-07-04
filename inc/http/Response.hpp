#pragma once

#include <Request.hpp>
#include <ServerParser.hpp>
#include <sstream>
#include <string>
#include <map>
#include <iostream>

struct CGIState
{
  bool          isCgi;
  pid_t         cgiPid;
  int           pipeFd;
  int           clientFd;
  int           stdinFd;
  std::string   stdinBody;
  size_t        stdinOffset;
  std::string   version;
  std::string   output;
  time_t        start;

    CGIState() : isCgi(false), cgiPid(-1), pipeFd(-1), clientFd(-1),
                 stdinFd(-1), stdinOffset(0), start(0) {}
};

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
        void setclientFd(int socket_Fd);
        void setVersion(const std::string& version);
        void setHeader(const std::string& header, const std::string& value);
        void setResponseData(int error, const std::string& reasonPhrase, const std::string& body);
        void setCGIState(pid_t pid, int pipeFd, int clientFd);
        void setCGIStdin(int inFd);

        // Getters
        const CGIState& getCGIState() const;
        int getCGIStdin() const;
        const std::string& getVersion() const;
        int getStatus() const;

    private:

        int         _errorCode;
        int         _cgiInFd;
        CGIState    _cgiState;
        std::string _version;
        std::string _body;
        std::string _reasonPhrase;
        std::map<std::string, std::string>  _headers;
};
