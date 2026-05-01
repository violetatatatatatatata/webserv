#include "CGIHandler.hpp"
#include "ErrorHandler.hpp"
#include <Request.hpp>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>

CGIHandler::CGIHandler(const Request& request, const Location* location, const ServerConfig& server, const std::string& extension) :
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
char** CGIHandler::buildArgv() const
{
    char** argv = new char*[3];

    argv[0] = const_cast<char*>(_binName.c_str());
    argv[1] = const_cast<char*>(_scriptPath.c_str());
    argv[2] = NULL;

    return argv;
}

std::vector<std::string> CGIHandler::buildEnv() const
{
    std::vector<std::string> env;
    
    env.push_back("REQUEST_METHOD=" + _request.getMethod());
    env.push_back("QUERY_STRING=");
    env.push_back("CONTENT_LENGTH=" + _request.getHeader("Content-Length"));
    env.push_back("SCRIPT_FILENAME=" + _scriptPath);

    return env;
}

char** CGIHandler::castEnv() const
{
    std::vector<std::string> envVars = buildEnv();    

    char** envArray = new char*[envVars.size() + 1];
    
    for (size_t i = 0; i < envVars.size(); ++i)
    {
        envArray[i] = const_cast<char*>(envVars[i].c_str());
    }

    envArray[envVars.size()] = NULL;
    return envArray;
}

void CGIHandler::internalError(const std::string& msg, Response& response) const
{
    ErrorHandler(500, _request, _server, response);
    perror(msg.c_str());
    return ;
} 

std::string CGIHandler::getDir() const
{
    size_t pos = _scriptPath.find_last_of('/');

    if (pos == std::string::npos)
        return ".";

    return _scriptPath.substr(0, pos);
}

void CGIHandler::executeCGI() const
{
    char* binPath = (char*)_binPath.c_str();
    char** argv = buildArgv();
    char** env = castEnv();
    
    chdir(getDir().c_str());
    execve(binPath, argv, env);

    perror("execve failed");
    exit(EXIT_FAILURE);
} 

static void receiveCgiData()
{

}

void CGIHandler::handleRequest(Response& response)
{
    std::cout << "CGI request !" << std::endl;

    int fd[2];
    if (pipe(fd) == -1)
        internalError("pipe", response);

    pid_t pid = fork();
    if (pid == -1)
        internalError("fork", response);

    if (pid == 0)
    {
        executeCGI();
    }
    else
    {
        receiveCgiData();
    }
}