#include "HandlerFactory.hpp"
#include "ServerParser.hpp"
#include "StaticHandler.hpp"
#include "ErrorHandler.hpp"

// Functions
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

/*static bool isRegularFile(const std::string& path)
{
    struct stat s;

    if (stat(path.c_str(), &s) != 0)
        return false;

    return S_ISREG(s.st_mode);
}*/

static int isRegularFile(const std::string& path)
{
    struct stat s;

    if (stat(path.c_str(), &s) != 0)
    {
        if (errno == EACCES)
            return 403;
        if (errno == ENOENT || errno == ENOTDIR)
            return 404;
        return 500;
    }

    if (!S_ISREG(s.st_mode))
        return 403;

    return 0;
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
/*static const std::vector<std::string>& getCorrectIndex(const LocationParser* location, const ServerParser& server)
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

static const std::string& getCorrectIndex(const LocationParser* location, const ServerParser& server)
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

// Methods
HttpHandler* HandlerFactory::create(const Request& request, const LocationParser* location, const ServerParser& server)
{
    // 1. REDIRECT
    if (location && !location->getRedirect().empty())
        return new RedirectHandler(request, location, server);

    std::string path = resolvePath(request, location, server); // directory traversal attack!!
    std::string ext;
    int errorStatus = isRegularFile(path);
    std::cout << "Path: " << path << std::endl;
    
    // 2. CGI
    if (location && isCgiRequest(location, path, ext) && errorStatus == 0)// && isRegularFile(path) == 0)
        return new CGIHandler(request, location, server, path, ext);

    // 3. FILE or DIRECTORY
    if (isDirectory(path))
    {
        std::string index = getCorrectIndex(location, server);
        std::string indexPath = getIndexPath(path, index);

        if (!indexPath.empty())
            return new StaticHandler(request, location, server, indexPath);

        if (location && location->getAutoindex())
            return new AutoIndexHandler(request, location, server, path);

        return new ErrorHandler(403, request, server);
    }

    // 4. REGULAR FILE
    if (isRegularFile(path))
        return new StaticHandler(request, location, server, path);
   
    std::cout << "Error: " << errorStatus << std::endl;
    if (errorStatus == 0)
        return new ErrorHandler(404, request, server);
    else
        return new ErrorHandler(errorStatus, request, server);
}