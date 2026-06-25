#include "HandlerFactory.hpp"
#include "ServerParser.hpp"
#include "StaticHandler.hpp"
#include "ErrorHandler.hpp"

static bool isCgiRequest(const LocationParser* loc, const std::string& path, std::string& ext)
{
    if (!loc)
        return false;

    std::string::size_type dot = path.find_last_of('.');
    if (dot != std::string::npos)
    {
        std::string tmp_ext = path.substr(dot);
        const std::vector<std::string>& cgiExt = loc->getCgiExt();

        for (std::vector<std::string>::const_iterator it = cgiExt.begin(); it != cgiExt.end(); it++)
        {
            if (tmp_ext == *it)
            {
                ext = tmp_ext;
                return true;
            }
        }
    }

    return false;
}

static std::string joinPath(const std::string& dir, const std::string& file)
{
    if (dir.empty())
        return file;
    if (file.empty())
        return dir;

    if (dir[dir.size() - 1] == '/')
        return dir + file;

    return dir + "/" + file;
}

static const std::string& getCorrectIndex(const LocationParser* location, const ServerParser& server)
{
    if (!location || location->getIndex().empty())
        return server.getIndex();
    else
        return location->getIndex();
}

static bool isDirectory(const std::string& path)
{
    struct stat s;

    if (stat(path.c_str(), &s) != 0)
        return false;

    return S_ISDIR(s.st_mode);
}

static std::string resolvePath(
    const Request& request,
    const LocationParser* location,
    const ServerParser& server)
{
    const std::string& uri = request.getURI();

    if (!location || location->getRoot().empty())
        return server.getRoot() + uri;

    std::string relativePath = uri;
    const std::string& locationPath = location->getPath();

    if (relativePath.compare(0, locationPath.size(), locationPath) == 0)
        relativePath.erase(0, locationPath.size());

    return location->getRoot() + relativePath;
}

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

static int isFileInError(int mode, const std::string file)
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

HttpHandler* HandlerFactory::create(const Request& request, const LocationParser* location, const ServerParser& server)
{
    if (location && !location->getRedirect().empty())
        return new RedirectHandler(request, location, server);

    std::string path = resolvePath(request, location, server);
    std::string ext;

    if (location && isCgiRequest(location, path, ext))
        return new CGIHandler(request, location, server, path, ext);

    if (isDirectory(path))
    {
        std::string index = getCorrectIndex(location, server);
        std::string indexPath = joinPath(path, index);
        int error_code = isFileInError(R_OK, indexPath);

        if (!indexPath.empty() && error_code == 0)
            return new StaticHandler(request, location, server, indexPath);

        if (location && location->getAutoindex())
            return new AutoIndexHandler(request, location, server, path);

        return new ErrorHandler(error_code, request, server);
    }

    return new StaticHandler(request, location, server, path);
}
