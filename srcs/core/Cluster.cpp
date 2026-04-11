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
		
		
		
		}
		else { //Si el puerto ya esta en uso.


		}
	
	
	}

}