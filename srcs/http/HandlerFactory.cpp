#include "HandlerFactory.hpp"
#include "ServerConfig.hpp"
#include "StaticHandler.hpp"
#include "ErrorHandler.hpp"

// Functions
static bool isCgiRequest(const Request& req, const Location* loc, const std::string& path)
{
    if (!loc)
        return false;

    // case 1 : check the cgi info in Location (cgi-bin)
    /*if (!loc->getCgiInfo().empty())
    {
        if (path.find(loc->getCgiInfo()) == 0)
            return true;
    }

    // case 2 : CGI extension
    std::string::size_type dot = path.find_last_of('.');
    if (dot != std::string::npos)
    {
        std::string ext = path.substr(dot);

        const std::vector<std::string>& cgiExt = loc->getCgiExtensions();
        for (size_t i = 0; i < cgiExt.size(); i++)
        {
            if (cgiExt[i] == ext)
                return true;
        }
    }*/
    (void)path;
    (void)req;
    return false;
}

static bool isRegularFile(const std::string& path)
{
    struct stat s;

    if (stat(path.c_str(), &s) != 0)
        return false;

    return S_ISREG(s.st_mode);
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

// Multi index Version
/*static const std::vector<std::string>& getCorrectIndex(const Location* location, const ServerConfig& server)
{
    if (location->getIndex().empty())
        return server->getIndex();
    else
        return location->getIndex();
}

static std::string getIndexFile(
    const std::string& dirPath,
    const std::vector<std::string>& indexList)
{
    if (indexList.empty())
        return "";

    for (size_t i = 0; i < indexList.size(); i++)
    {
        std::string fullPath = joinPath(dirPath, indexList[i]);

        if (isRegularFile(fullPath))
            return indexList[i];
    }

    return "";
}*/

static const std::string& getCorrectIndex(const Location* location, const ServerConfig& server)
{
    if (!location || location->getIndex().empty())
        return server.getIndex();
    else
        return location->getIndex();
}

static std::string getIndexPath(
    const std::string& dirPath,
    const std::string& index)
{
    if (index.empty())
        return "";

    std::string fullPath = joinPath(dirPath, index);

    if (isRegularFile(fullPath))
            return fullPath;

    return "";
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
    const Location* location,
    const ServerConfig& server)
{
    if (!location || location->getRoot().empty())
        return server.getRoot() + request.getURI();

    return location->getRoot() + request.getURI();
}

// Methods
HttpHandler* HandlerFactory::create(
    const Request& request,
    const Location* location,
    const ServerConfig& server,
    Response& response)
{
    // 1. REDIRECT
    if (location && !location->getRedirect().empty())
        return new RedirectHandler(request, location, server);

    std::string path = resolvePath(request, location, server); // directory traversal attack!!
    
    // 2. CGI
    if (location && isCgiRequest(request, location, path))
        return new CGIHandler(request, location, server, path);
    return new CGIHandler(request, location, server, path);

    // 3. FILE or DIRECTORY
    if (isDirectory(path))
    {
        std::string index = getCorrectIndex(location, server);
        std::string indexPath = getIndexPath(path, index);

        if (!indexPath.empty())
            return new StaticHandler(request, location, server, indexPath);

        if (location && location->getAutoindex())
            return new AutoIndexHandler(request, location, server, path);

        return new ErrorHandler(403, request, server, response);
    }

    // 4. REGULAR FILE
    if (isRegularFile(path))
        return new StaticHandler(request, location, server, path);

    return new ErrorHandler(404, request, server, response);
}