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
#include <Client.hpp>
#include <Request.hpp>
#include <Response.hpp>
#include <ServerParser.hpp>

class Client {

	public:
	enum ParseState {
		READING_HEADERS,
		READING_BODY,
		READY
	};

	private:
		int         _fd;
		int			_serverFd;

		std::string _requestBuffer;

		std::string	_responseBuffer;
		size_t		_responseOffset;

		ParseState _parseState;
		size_t _headerEndPos;
		size_t _expectedBodySize;
		bool _bodyTooLarge;

		time_t _lastActivity;

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

		bool isRequestComplete();

		void		setResponse(const std::string& response);
		bool 		hasDataToSend() const;
		const char*	getWritePtr() const;
		size_t		getWriteRemaining() const;
		void		advanceWriteOffset(size_t bytes);
		bool		isResponseFullySent() const;

		bool isBodyTooLarge() const;

		time_t getLastActivity() const;
		void updateActivity();

	private:
		void processHeaders();
		void extractContentLength();
		void processBody();
};

#endif
