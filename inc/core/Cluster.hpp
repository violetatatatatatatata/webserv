#ifndef SERVER_HPP
# define SERVER_HPP

 #include <Webserv.hpp>

//class Client; //TODO

class Cluster {
	private:
	std::map<int, std::vector<ServerConfig> >	_configs;    
		std::vector<struct pollfd>					_fds;        
		std::map<int, std::vector<ServerConfig> >	_serverFds;  
		//std::map<int, Client>						_clientsFds; 
	
	public:

		Cluster();
		Cluster(const Cluster & other);
		~Cluster();
		Cluster & operator=(const Cluster & other);
		
		Cluster(const std::map<int, std::vector<ServerConfig> >& servers);

		void run();

	private:
		void init();
		void acceptClient(int serverFd);
		//void handleClient(Client& client);
		void disconnectClient(int fd);
};
#endif