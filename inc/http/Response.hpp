#pragma once

class Response
{
    public:

        Response();
        Response(const Response& other);
        ~Response();
        Response& operator=(const Response& other);

        // Getters
        int getSocketFd() const;

        // Setters
        void setSocketFd(int socketFd);

    private:

        int _socketFd;
};