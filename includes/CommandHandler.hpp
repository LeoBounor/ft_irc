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

};

#endif