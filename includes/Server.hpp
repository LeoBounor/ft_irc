#ifndef SERVER_HPP
#define SERVER_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <sstream>
#include <poll.h>
#include <vector>
#include <cerrno>
#include <map>

# include "ft_irc.hpp"

# define DEFAULT_PORT 6667
# define DEFAULT_SERVER_NAME "irc.42.fr"

class Server
{
	private:
		std::string 				_port;
		std::string					_password;
		int							_fd_socket;
		bool						_running;
		std::string					_server_name;
		std::string					_server_time;
		std::vector<pollfd> 		_pollfds;
		std::vector<Channel *>		_channels;
		std::map<size_t, Client *>	_clients;
		CommandHandler				_command_handler;

	public:
		Server(std::string const &port, std::string const &password);
		~Server();
		void						run();
		void						close_server();
		int							create_socket();
		Channel 					*create_channel(std::string const &name, std::string const &password);
		bool						is_running();
		bool						hasPassword();
		const std::string			getPassword();
		const std::string			getServerName();
		std::vector<Channel *>		getChannels();
		Channel 					*getChannel(std::string const &name);
		std::vector<Client *>		getClients();
		Client 						*getClient(std::string const &name);

};

#endif