#include "CGIHandler.hpp"
#include "ErrorHandler.hpp"
#include <Request.hpp>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

CGIHandler::CGIHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& extention) :
HttpHandler(request, location, server), _ext(extension)
{
    if (_ext == ".php")
    {
        _binPath = "/usr/bin/php-cgi";
        _binName = "php-cgi";
    }

    if (_ext == ".py")
    {
        _binPath = "/usr/bin/python3";
        _binName = "python3";
    }

}

CGIHandler::~CGIHandler() {}

// Methods
char** CGIHandler::buildBinary()
{
    //char* path = _path.find_last_of('/').c_str();

    return NULL;
}

char** CGIHandler::buildEnv()
{
    _env.push_back("REQUEST_METHOD=" + std::to_string(_request.getMethod()));
    _env.push_back("QUERY_STRING=");
    _env.push_back("CONTENT_LENGTH=" + std::to_string(_request.getBody().size()));
    _env.push_back("SCRIPT_FILENAME=");

    return NULL;
}

void CGIHandler::internalError(const std::string& msg) const
{
    ErrorHandler(500, _request, _server, response);
    perror(msg);
    return ;
} 

static void executeCGI()
{
    char* path = (char*)_path.c_str();
    char** bin = buildBinary();
    char** env = buildEnv();
    
    execve(path, bin, env);
} 

static void receiveCgiData()
{

} 

void CGIHandler::handleRequest(Response& response)
{
    std::cout << "CGI request !" << std::endl;

    int fd[2];
    if (pipe(fd) == -1)
    {
        internalError("pipe");
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        internalError("fork");
    }

    if (pid == 0)
    {
        executeCGI();
    }
    else
    {
        receiveCgiData();
    }

    (void)response;
}