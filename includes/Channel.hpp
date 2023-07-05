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

};

#endif