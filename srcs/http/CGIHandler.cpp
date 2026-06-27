#include "CGIHandler.hpp"
#include "ErrorHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "LocationParser.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

//Functions
static size_t findCgiIndex(const LocationParser* location, const std::string& extension)
{
    const std::vector<std::string>& exts = location->getCgiExt();

    for (size_t i = 0; i < exts.size(); ++i)
    {
        if (exts[i] == extension)
            return i;
    }
    return std::string::npos;
}

CGIHandler::CGIHandler(const Request& request, const LocationParser* location, const ServerParser& server, const std::string& url, const std::string& extension) :
HttpHandler(request, location, server), _url(url), _ext(extension)
{
    const std::vector<std::string>& bins = location->getCgiPath();

    size_t idx = findCgiIndex(location, extension);

    if (idx == std::string::npos || idx >= bins.size())
        throw std::runtime_error("No CGI handler for extension");

    _binPath = bins[idx];

    size_t slash = _binPath.find_last_of('/');
    if (slash != std::string::npos)
        _binName = _binPath.substr(slash + 1);
    else
        _binName = _binPath;
}

CGIHandler::~CGIHandler() {}

// Methods
std::string CGIHandler::findScript() const
{
    std::size_t lastSlash = 0;
    std::size_t pos = 0;

    while ((pos = _url.find('/', lastSlash)) != std::string::npos)
    {
        lastSlash = pos + 1;
    }

    std::size_t start = 0;

    while (start < _url.size())
    {
        std::size_t slash = _url.find('/', start);
        std::string segment;

        if (slash == std::string::npos)
            segment = _url.substr(start);
        else
            segment = _url.substr(start, slash - start);

        if (segment.size() >= _ext.size() && segment.substr(segment.size() - _ext.size()) == _ext)
            return segment;

        if (slash == std::string::npos)
            break;

        start = slash + 1;
    }

    return "";
}

ParsedURL CGIHandler::parseURL() const
{
    ParsedURL res;

    std::string script = findScript();
    if (script.empty())
        return res;

    std::size_t pos = _url.find(script);
    if (pos == 0)
        res.directory = ".";
    else
        res.directory = _url.substr(0, pos);

    res.script = script;

    std::size_t start = pos + script.length();
    if (start < _url.size())
        res.path = _url.substr(start);
    else
        res.path = "";

    if (!res.path.empty() && res.path[0] != '/')
        res.path = "";

    return res;
}

char** CGIHandler::buildArgv(const std::string& script) const
{
    char** argv = new char*[3];

    argv[0] = const_cast<char*>(_binName.c_str());
    argv[1] = const_cast<char*>(script.c_str());
    argv[2] = NULL;

    return argv;
}

std::vector<std::string> CGIHandler::buildEnv(const ParsedURL& urlInfo) const
{
    std::vector<std::string> env;
    
    env.push_back("SERVER_PROTOCOL=" + _request.getVersion());
    env.push_back("REQUEST_METHOD=" + _request.getMethod());
    env.push_back("SCRIPT_FILENAME=" + urlInfo.script);
    env.push_back("CONTENT_LENGTH=" + _request.getHeader("Content-Length"));
    env.push_back("PATH_INFO=" + urlInfo.path);

    return env;
}

char** CGIHandler::castEnv(const ParsedURL& urlInfo) const
{
    std::vector<std::string> envVars = buildEnv(urlInfo);

    char** envArray = new char*[envVars.size() + 1];

    for (size_t i = 0; i < envVars.size(); ++i)
    {
        envArray[i] = strdup(envVars[i].c_str());
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

void CGIHandler::executeCGI() const
{
    ParsedURL urlInfo = parseURL();

    char* binPath = (char*)_binPath.c_str();
    char** argv = buildArgv(urlInfo.script);
    char** env = castEnv(urlInfo);

    chdir(urlInfo.directory.c_str());
    execve(binPath, argv, env);

    perror("execve failed");
    exit(EXIT_FAILURE);
}

void CGIHandler::handleFd(int fd[2], Response& response) const
{
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    if (_request.getMethod() == "GET" || _request.getMethod() == "DELETE")
    {
        int devNull = open("/dev/null", O_RDONLY);
        dup2(devNull, STDIN_FILENO);
        close(devNull);
    }
    else if (_request.getMethod() == "POST")
    {
        int stdin_fd[2];
        if (pipe(stdin_fd) == -1)
            internalError("pipe", response);
            
        write(stdin_fd[1], _request.getBody().c_str(), _request.getBody().size());
        close(stdin_fd[1]);
        dup2(stdin_fd[0], STDIN_FILENO);
        close(stdin_fd[0]);
    }
}

void CGIHandler::handleRequest(Response& response)
{
    int res = HttpHandler::isFileInError(R_OK, _url);
    if (res != 0)
    {
        ErrorHandler(res, _request, _server, response);
        return ;
    }
    
    int fd[2];
    if (pipe(fd) == -1)
        internalError("pipe", response);

    pid_t pid = fork();
    if (pid == -1)
        internalError("fork", response);

    if (pid == 0)
    {
        handleFd(fd, response);   
        executeCGI();
    }
    
    close(fd[1]);
    response.setCGIState(pid, fd[0], _request.getclientFd());

    /*time_t start = std::time(NULL);
    char buf[1024];
    std::string data;
    ssize_t n;

    fcntl(fd[0], F_SETFL, O_NONBLOCK);

    while (true)
    {
        if (std::time(NULL) - start > TIMEOUT)
        {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            close(fd[0]);
            ErrorHandler(504, _request, _server, response);
            return;
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd[0], &readfds);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;

        int ret = select(fd[0] + 1, &readfds, NULL, NULL, &tv);

        if (ret < 0)
            break;
        else if (ret == 0)
            continue;
    
        n = read(fd[0], buf, sizeof(buf));
        if (n > 0)
            data.append(buf, n);
        else
            break;
    }

    close(fd[0]);
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        ErrorHandler(500, _request, _server, response);
    else
        response.setResponseData(200, "OK", data);*/
}