#include "Response.hpp"

Response::Response() : _error_code(0) {}
Response::~Response() {}

Response::Response(const Response& other)
{
  *this = other;
}


Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
      _socket_Fd = other._socket_Fd;
      _error_code = other._error_code;
    }
    
    return *this;
}

// Methods
void Response::fillBody()
{
  
}


// Setters
void Response::setError(int error_code)
{
	_error_code = error_code;
}
