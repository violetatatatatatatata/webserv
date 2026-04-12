#pragma once

class Response
{
    public:

        Response();
        Response(const Response& other);
        ~Response();
        Response& operator=(const Response& other);

        // Methods
        void fillBody();

        // Getters
        int getSocketFd() const;

        // Setters
        void setSocketFd(int socket_Fd);
        void setError(int error_code);

    private:

        int _socket_Fd;
        int _error_code;
};