#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Channel
{
	private:
		std::string				_name;
		std::string				_password;
		std::vector<Client *>	_users;

	public:
		Channel(std::string name, std::string password);
		~Channel();
		std::string					getName() const { return _name; }
		std::string 				getPassword() const { return _password; }
		std::vector<std::string> 	getNicknames();
		bool						hasClient(Client *client);
		
		void broadcast(std::string const &message, Client *sender = NULL);
		void addUser(Client *user);
		void removeUser(Client *user);
		void kickUser(Client *kicker, Client *target, std::string reason);
};

#endif