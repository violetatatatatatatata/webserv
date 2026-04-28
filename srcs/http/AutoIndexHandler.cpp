#include "AutoIndexHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include <iostream>
#include <sstream>
#include <dirent.h>

AutoIndexHandler::AutoIndexHandler(
    const Request& request,
    const Location* location,
    const ServerConfig& server,
    const std::string& directory) : HttpHandler(request, location, server), _directory(directory)
{
}

AutoIndexHandler::~AutoIndexHandler() {}

AutoIndexHandler::AutoIndexHandler(const AutoIndexHandler& other) : HttpHandler(other)
{
}

// Methods
void AutoIndexHandler::handleRequest(Response& response)
{
    std::cout << "Autoindex request !" << std::endl;

    response.setVersion(_request.getVersion());
    DIR* dir = opendir(_directory.c_str());
    if (!dir)
    {
        return ; //error TODO
    }

    std::stringstream html;

    html << "<html>\n";
    html << "<head>\n";
    html << "\t<title>Index of " << _request.getURI() << "</title>\n";  // <h1> retiré du title
    html << "</head>\n";
    html << "<body style=\"background-color: rgb(238, 225, 200)\">\n";
    html << "\t<h1>Index of " << _request.getURI() << "</h1>\n";
    html << "\t<hr>\n";
    html << "\t<ul>\n";

    struct dirent* file;
    while ((file = readdir(dir)) != NULL)
    {
        std::string name = file->d_name;

        if (name == "." || name == "..")
            continue;

        html << "\t\t<li><a href=\"" << _request.getURI() << '/' << name << "\">" << name << "</a></li>\n";
    }

    html << "\t</ul>\n";
    html << "\t<hr>\n";
    html << "</body>\n";
    html << "</html>";

    std::cout << html << std::endl;
    response.setResponseData(200, "OK", html.str()); 
}