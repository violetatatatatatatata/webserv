#include <Webserv.hpp>

//Orthodox Canonical Form

Cluster::Cluster(const std::vector<ServerConfig>& servers) : _configs(servers) {
	print_msg("Initializing cluster", DEBUG);
	init();
}

Cluster::Cluster(const Cluster & other) : _configs(other._configs) {
	*this = other;
}

Cluster::~Cluster() {
	print_msg("Destrutor cluster", DEBUG);
	// TODO: close FD's
}

Cluster & Cluster::operator=(const Cluster & other) {
	if (this != &other) {
		this->_fds = other._fds;
		this->_serverFds = other._serverFds;
		//this->_clientsFds = other._clientsFds;
	}
	return *this;
}



void Cluster::init() {

	std::map<int, int> portToFd;

	for (size_t i = 0; i < _configs.size(); ++i) {
		int port = _configs[i].getPort();

		if (portToFd.find(port) == portToFd.end()) {
		
		int serverFd = socket(AF_INET, SOCK_STREAM, 0);
			if (serverFd < 0) {
				print_msg("Failed to create socket", FATAL);
				return; 
				//std::exception??
			}

			int opt = 1;
			if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
				print_msg("Failed to set SO_REUSEADDR", WARN);
			}
			
			struct sockaddr_in addr;
			std::memset(&addr, 0, sizeof(addr)); 
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(port);

			if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
				print_msg("Bind failed", FATAL); 
				close(serverFd);
				//std::exception??
				return;
			}

			if (listen(serverFd, SOMAXCONN) < 0) {
				print_msg("Listen failed", FATAL);
				close(serverFd);
				return;
			}

			portToFd[port] = serverFd;
			this->_serverFds[serverFd].push_back(_configs[i]);

			struct pollfd pfd;
			pfd.fd = serverFd;
			pfd.events = POLLIN;
			pfd.revents = 0;
			this->_fds.push_back(pfd);

			print_msg("Socket connected", SUCCESS);

		} else { 

			this->_serverFds[portToFd[port]].push_back(_configs[i]);
			
			print_msg("server add", INFO);
		}
	}

}
