/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datienza <datienza@student.42barcelo>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 20:18:44 by datienza          #+#    #+#             */
/*   Updated: 2026/04/30 20:18:46 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

 # include <Webserv.hpp>
 # include <ServerParser.hpp>
 # include <Client.hpp>
 # include <Response.hpp>
 # include <Request.hpp>
 # include <HandlerFactory.hpp>

class Server {
	private:
		int							_fd;
		int							_port;
		std::vector<ServerParser>	_configs;

	public:

		Server();
		Server(const Server& other);
		Server& operator=(const Server& other);
		~Server();

		Server(int port, const std::vector<ServerParser>& configs);

		bool init();

		bool createSocket();
		bool setOptions();
		bool bindListen();

		int								 getFd() const;
		int								 getPort() const;
		const std::vector<ServerParser>& getConfigs() const;

};

#endif
