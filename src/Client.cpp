/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:50 by Leo               #+#    #+#             */
/*   Updated: 2023/07/08 19:44:34 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Client::Client(int fd, int port, std::string hostname)
    : _fd(fd), _port(port), _hostname(hostname), _status(0), _channels(nullptr)
{
}

Client::~Client()
{
}

std::string Client::getClientPrefix() const
{
    std::string prefix;

    if (!_nickname.empty())
        prefix += _nickname + "!";

    if (!_username.empty())
        prefix += _username + "@";
        

    if (!_hostname.empty())
        prefix += _hostname;

    return prefix;
}

void Client::send(std::string const &message) const
{
    ssize_t bytesSent = ::send(this->_fd, message.c_str(), message.size(), 0);
    if (bytesSent == -1)
        perror("Erreur lors de l'envoi des données");
}

void Client::sendReply(std::string const &message)
{
    this->send(":" + this->getClientPrefix() + " " + message);
}

void Client::welcome()
{
    this->sendReply(RPL_WELCOME(this->_nickname, this->_nickname));
    printMsg(this->_nickname + " connected");
}

void Client::disconnect()
{
    if (this->_channels == nullptr)
        return;
    this->_channels->broadcast(RPL_PART(this->getClientPrefix(), this->_channels->getName()));
    this->_channels = nullptr;
    printMsg(this->_nickname + " has left the channel" + this->_channels->getName());
    this->_channels->removeUser(this);
}

void Client::joinChannel(Channel *channel)
{
	if (channel)
	{
		// Vérifier si le client est déjà dans le canal
		if (channel->hasClient(this))
			return;
		channel->addUser(this);
		this->_channels = channel;
		std::vector<std::string> users = channel->getNicknames();
		std::string users_string;
		for (const std::string& user : users)
		{
			users_string += user + " ";
		}

		sendReply(RPL_NAMREPLY(this->_nickname, channel->getName(), users_string));
		sendReply(RPL_ENDOFNAMES(this->_nickname, channel->getName()));

		channel->broadcast(RPL_JOIN(getClientPrefix(), channel->getName()));
		printMsg(this->_nickname + " has joined channel " + channel->getName());
	}
}

