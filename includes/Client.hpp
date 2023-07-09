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
		Client(int fd, std::string hostname, int port);
		~Client();
		int					getFd() const { return _fd; }
		int					getPort() const { return _port; }
		std::string			getHostname() const { return _hostname; }
		std::string			getNickname() const { return _nickname; }
		std::string			getUsername() const { return _username; }
		std::string			getRealname() const { return _realname; }
		int					getStatus() const { return _status; }
		Channel				*getChannels() const { return _channels; }
		std::string			getClientPrefix() const;

		void				setFd(int fd) { _fd = fd; }
		void				setPort(int port) { _port = port; }
		void				setHostname(std::string hostname) { _hostname = hostname; }
		void				setNickname(std::string nickname) { _nickname = nickname; }
		void				setUsername(std::string username) { _username = username; }
		void				setRealname(std::string realname) { _realname = realname; }
		void				setStatus(int status) { _status = status; }
		void				setChannels(Channel *channels) { _channels = channels; }

		void				send(std::string const &message) const;
		void				sendReply(std::string const &message);
		void				welcome();
		void				joinChannel(Channel *channel);
		void				disconnect();
};

#endif