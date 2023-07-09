#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "ft_irc.hpp"

class Command
{
	protected:
		Server 			*_server;
		int				_isAuth;
	
	public:
		Command(Server *server, int isAuth = 0) : _server(server), _isAuth(isAuth) {};
		virtual ~Command();
		virtual void execute(Client *client, std::vector<std::string> args) = 0;
		int		getIsAuth() const { return _isAuth; };
};



#endif