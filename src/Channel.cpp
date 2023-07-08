/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:44 by Leo               #+#    #+#             */
/*   Updated: 2023/07/08 19:44:12 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Channel::Channel(std::string name, std::string password)
	: _name(name), _password(password)
{
}

Channel::~Channel()
{
}
std::vector<std::string> Channel::getNicknames()
{
	std::vector<std::string> nicknames;
	for (Client* user : _users)
	{
		nicknames.push_back(user->getNickname());
	}
	return nicknames;
}

bool Channel::hasClient(Client *client)
{
    return std::find(_users.begin(), _users.end(), client) != _users.end();
}

void Channel::broadcast(std::string const &message, Client *sender)
{
	for (Client* user : _users)
	{
		// Exclure l'expéditeur du message s'il est fourni
		if (sender && user == sender)
			continue;
		user->send(message);
	}
}

void Channel::addUser(Client *user)
{
	this->_users.push_back(user);
	if (this->_users.size() == 1)
		user->setStatus(2);
}

void Channel::removeUser(Client *client)
{
	auto it = std::find(_users.begin(), _users.end(), client);
	if (it != _users.end())
	{
		_users.erase(it);
	}
	client->setChannels(nullptr);
	if (client->getStatus() == 2)
	{
		client->setStatus(1);
		if (!_users.empty())
		{
			_users[0]->setStatus(2);
			std::string msg = _users[0]->getNickname() + " is now the admin of " + _name;
			printMsg(msg);
		}
	}
}


void Channel::kickUser(Client *kicker, Client *target, std::string reason)
{
	if (target)
	{
		// Envoyer un message de kick à tous les utilisateurs du canal
		broadcast(RPL_KICK(kicker->getNickname(), _name, target->getNickname(), reason), kicker);
		// Supprimer le client cible du canal
		removeUser(target);

		std::string message = kicker->getNickname() + " kicked " + target->getNickname() + " from " + _name;
		printMsg(message);
	}
}
