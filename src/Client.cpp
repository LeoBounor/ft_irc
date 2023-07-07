/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:50 by Leo               #+#    #+#             */
/*   Updated: 2023/07/07 13:31:21 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

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

void Client::send(std::string message)
{
    ssize_t bytesSent = ::send(_fd, message.c_str(), message.size(), 0);
    if (bytesSent == -1)
        perror("Erreur lors de l'envoi des données");
}

