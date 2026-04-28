#pragma once

#include "HttpHandler.hpp"

class ErrorHandler : public HttpHandler
{
  public:
    ErrorHandler();
    ErrorHandler(const ErrorHandler& other);
    ~ErrorHandler();
    ErrorHandler& operator=(const ErrorHandler& other);

  private:
    
};