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
#include <limits.h>

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

static std::string toHttpEnvName(const std::string& header)
{
    std::string result = "HTTP_";
    for (size_t i = 0; i < header.size(); ++i)
    {
        char c = header[i];
        if (c == '-')
            result += '_';
        else
            result += std::toupper(static_cast<unsigned char>(c));
    }
    return result;
}

std::vector<std::string> CGIHandler::buildEnv(const ParsedURL& urlInfo) const
{
    std::vector<std::string> env;

    std::string pathInfo = urlInfo.path.empty() ? "/" : urlInfo.path;

    env.push_back("SERVER_PROTOCOL=" + _request.getVersion());
    env.push_back("REQUEST_METHOD=" + _request.getMethod());
    env.push_back("SCRIPT_FILENAME=" + urlInfo.script);
    std::string cl = _request.getHeader("Content-Length");
    if (cl.empty())
    {
        std::ostringstream oss;
        oss << _request.getBody().size();
        cl = oss.str();
    }
    env.push_back("CONTENT_LENGTH=" + cl);
    env.push_back("PATH_INFO=" + pathInfo);

    // Pass HTTP request headers as HTTP_* env vars and special CGI vars
    const std::map<std::string, std::string>& hdrs = _request.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = hdrs.begin(); it != hdrs.end(); ++it)
    {
        const std::string& name = it->first;
        if (name == "Content-Length")
            continue; // already set as CONTENT_LENGTH
        if (name == "Content-Type")
        {
            env.push_back("CONTENT_TYPE=" + it->second);
            env.push_back("HTTP_CONTENT_TYPE=" + it->second);
        }
        else
        {
            env.push_back(toHttpEnvName(name) + "=" + it->second);
        }
    }

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

    char resolvedBin[PATH_MAX];
    std::string binStr;
    if (realpath(_binPath.c_str(), resolvedBin) != NULL)
        binStr = resolvedBin;
    else
        binStr = _binPath;

    char** argv = buildArgv(urlInfo.script);
    char** env = castEnv(urlInfo);

    chdir(urlInfo.directory.c_str());
    execve(binStr.c_str(), argv, env);

    perror("execve failed");
    exit(EXIT_FAILURE);
}

void CGIHandler::handleFd(int fd[2], Response& response) const
{
    (void)response;
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    if (_request.getMethod() == "POST")
    {
        FILE* tmp = tmpfile();
        if (!tmp) { perror("tmpfile"); exit(EXIT_FAILURE); }

        const std::string& body = _request.getBody();
        size_t written = 0;
        while (written < body.size())
        {
            ssize_t ret = write(fileno(tmp), body.c_str() + written, body.size() - written);
            if (ret <= 0) break;
            written += (size_t)ret;
        }
        rewind(tmp);
        dup2(fileno(tmp), STDIN_FILENO);
        fclose(tmp);
    }
    else
    {
        int devNull = open("/dev/null", O_RDONLY);
        if (devNull != -1)
        {
            dup2(devNull, STDIN_FILENO);
            close(devNull);
        }
    }
}

void CGIHandler::handleRequest(Response& response)
{
    // Only check that the CGI binary exists, not the script (the binary handles missing scripts)
    int res = HttpHandler::isFileInError(X_OK, _binPath);
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