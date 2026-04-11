#include "Response.hpp"

Response::Response()
{
}

Response::Response(const Response& other)
{
  *this = other;
}

Response::~Response()
{
}

Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
      _socketFd = other._socketFd;
    }
    
    return *this;
}
