#ifndef FT_IRC_H
# define FT_IRC_H

# include <iostream>

# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include "CommandHandler.hpp"
# include "CommandReplies.hpp"
# include "Command.hpp"

std::string 				dateString(void);
void						printMsg(std::string const &msg);

#endif