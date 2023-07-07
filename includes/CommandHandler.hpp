#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

# include <string>

# include "ft_irc.hpp"

class CommandHandler
{
	private:
		Server 								*_server;
		std::map<std::string, Command *> 	_commands;
	
	public:
		CommandHandler(Server *server);
		~CommandHandler();

		int 		handle_command(Client *client, std::string &line);
};

#endif