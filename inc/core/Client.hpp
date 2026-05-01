/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datienza <datienza@student.42barcelo>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 23:13:49 by datienza          #+#    #+#             */
/*   Updated: 2026/04/21 23:13:52 by datienza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <Webserv.hpp>

class Client {
	private:
		int         _fd;
		int			_serverFd;

		std::string _requestBuffer;

		std::string	_responseBuffer;
		size_t		_responseOffset;

	public:
		//
		Client();
		Client(const Client& other);
		Client& operator=(const Client& other);
		~Client();

		Client(int fd, int serverFd);

		int getFd() const;
		int getServerFd() const;
		const std::string& getBuffer() const;


		void appendData(const char* buffer, ssize_t bytesRead);
		bool isHeaderComplete() const;

		void		setResponse(const std::string& response);
		bool 		hasDataToSend() const;
		const char*	getWritePtr() const;
		size_t		getWriteRemaining() const;
		void		advanceWriteOffset(size_t bytes);
		bool		isResponseFullySent() const;
};

#endif
