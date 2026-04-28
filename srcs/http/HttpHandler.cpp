#include "HttpHandler.hpp"
#include "ServerConfig.hpp"
#include "Request.hpp"
#include <sys/stat.h>
#include <cerrno>

HttpHandler::HttpHandler(const Request& request, const Location* location, const ServerConfig& server) : _request(request), _location(location), _server(server)
{
}

HttpHandler::HttpHandler(const HttpHandler& other) : 
_request(other._request), _location(other._location), _server(other._server)
{
}

HttpHandler::~HttpHandler(){}

// Methods
static int checkStat(struct stat& st, const std::string& file)
{
    if (stat(file.c_str(), &st) == -1)
    {
        switch(errno)
        {
            case ENOENT:
            case ENOTDIR:
                return 404;
            case EACCES:
                return 403;
            default:
                return 500;
        }
    }
    return 0;
}

int HttpHandler::isFileInError(int mode, const std::string file) const
{
    struct stat st;

    int res = checkStat(st, file);
    if (res != 0)
        return res;

    if (S_ISDIR(st.st_mode))
            return 404;

    if (access(file.c_str(), mode) == -1)
        return 403;

    return 0;
}