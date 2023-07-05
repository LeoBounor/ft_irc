#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class Client
{
	private:
		int				_fd;
		int				_port;
		std::string		_hostname;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		int				_status;
		Channel			*_channels;

	public:

};

#endif