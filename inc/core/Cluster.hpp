/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datienza <datienza@student.42barcelo>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 23:14:09 by datienza          #+#    #+#             */
/*   Updated: 2026/04/21 23:14:11 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

 #include <Webserv.hpp>

class Client;

class Cluster {
	private:
		std::vector<Server*> _servers;
		std::vector<struct pollfd> _fds;
		std::map<int, Client>      _clientsFds;

	public:
		Cluster();
		Cluster(const Cluster& other);
		~Cluster();
		Cluster& operator=(const Cluster& other);

		Cluster(const std::map<int, std::vector<ServerConfig> >& configs);

		void run();

	private:
		void    init(const std::map<int, std::vector<ServerConfig> >& configs);

		Server* findServer(int fd);

		void    acceptClient(int serverFd);
		void    handleClientData(int clientFd, size_t pollIndex);
		void    processHttpRequest(Client& client, int clientFd, size_t pollIndex);
		void    disconnectClient(int clientFd, size_t pollIndex);
};
#endif
