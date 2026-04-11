#ifndef SERVER_HPP
# define SERVER_HPP

 #include <Webserv.hpp>

class Client; //TODO

class Cluster {
	private:
		const std::vector<ServerConfig>&            _configs;    // Ref to servers
		std::vector<struct pollfd>                  _fds;        // array to poll
		std::map<int, std::vector<ServerConfig> >   _serverFds;  // fds connection socket -> virtual servers
		//std::map<int, Client>                       _clientsFds; // fds clients -> state
	
	public:

		Cluster();
		Cluster(const Cluster & other);
		~Cluster();
		Cluster & operator=(const Cluster & other);
		
		Cluster(const std::vector<ServerConfig>& servers);

	private:
		void init();
		void acceptClient(int serverFd);
		void handleClient(Client& client);
		void disconnectClient(int fd);
};
#endif