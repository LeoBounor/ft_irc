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
		std::string getName();
		std::string getPassword();
		
		void addUser(Client *user);
		void removeUser(Client *user);
		void kickUser(Client *kicker, Client *target, std::string reason);

};

#endif