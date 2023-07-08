/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:36 by Leo               #+#    #+#             */
/*   Updated: 2023/07/08 12:36:05 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Server::Server(std::string const &port, std::string const &password) :
	_port(port),
	_password(password),
	_fd_socket(this->create_socket()),
	_running(1),
	_server_name(DEFAULT_SERVER_NAME),
	_command_handler(new CommandHandler(this))
{
}

Server::~Server()
{
    std::vector<int> fds;

    for (const auto& clientPair : _clients)
        fds.push_back(clientPair.first);

    for (int fd : fds)
    {
        _clients[fd]->sendReply("Arrêt du serveur\n");
        handle_disconnect(fd);
    }
    for (Channel* channel : _channels)
        delete channel;
    delete _command_handler;
    if (_fd_socket != -1)
        close(_fd_socket);

    printMsg("Server Closed");
}

void Server::run()
{
    pollfd server_fd = { _fd_socket, POLLIN, 0 };
	_pollfds.push_back(server_fd);

	printMsg("Serveur en attente de connexions");

	while (is_running())
	{
		// Attente des événements
		if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
			throw std::runtime_error("Erreur lors du polling");

		// Gestion des événements
		for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
		{
			// Pas d'événements
			if (it->revents == 0)
				continue;
			// Connexion entrante
			if ((it->revents & POLLIN) == POLLIN)
			{
				if (it->fd == _fd_socket)
				{
					add_new_client();
					break;
				}
				if (handle_message(it->fd))
					break;
			}
			// Déconnexion
			if ((it->revents & POLLHUP) == POLLHUP)
			{
				handle_disconnect(it->fd);
				break;
			}
		}
	}
}

void Server::add_new_client()
{
	struct sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);
	std::string	ip_addr;

    // Accepter une nouvelle connexion
    int clientSockfd = accept(_fd_socket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSockfd == -1)
    {
        perror("Erreur lors de l'acceptation de la connexion");
        return;
    }
	ip_addr = inet_ntoa(clientAddr.sin_addr);
    Client* newClient = new Client(clientSockfd, ip_addr, ntohs(clientAddr.sin_port));
    _clients[clientSockfd] = newClient;
    _pollfds.push_back({ clientSockfd, POLLIN, 0 });
	printMsg(newClient->getNickname() + " vient de se connecter");
}

int Server::create_socket()
{
    // Création du socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse et du port du serveur
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(_port));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Liaison du socket à l'adresse et au port du serveur
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Erreur lors de la liaison du socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Mise en écoute du socket
    if (listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Erreur lors de la mise en écoute du socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

int	Server::handle_message(int fd)
{
    Client*			client = _clients[fd];
    std::string		message = this->receive_data(fd);

    if (message.empty())
    {
        handle_disconnect(fd);
        return (1);
    }
	// Code pour traiter le message
	if (this->_command_handler->handle_command(client, message))
	{
		handle_disconnect(client->getFd());
		return (1);
	}
    return (0);
}

std::string Server::receive_data(int fd)
{
	std::string		data;
	const int		buffer_size = 1024;
	char			buffer[buffer_size];
	ssize_t			bytesRead = recv(fd, buffer, buffer_size - 1, 0);

	if (bytesRead == -1)
	{
		perror("Erreur lors de la réception des données");
		return data;
	}
	if (bytesRead == 0)
		return data;
	buffer[bytesRead] = '\0';
	data = buffer;
	return data;
}

void Server::handle_disconnect(int fd)
{
	close(fd);

	// Suppression du client de la map _clients
	auto it = _clients.find(fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}

	// Suppression du file descriptor du client des pollfds
	auto pollIt = std::find_if(_pollfds.begin(), _pollfds.end(), [fd](const pollfd& pfd) {
		return pfd.fd == fd;
	});
	if (pollIt != _pollfds.end())
	{
		_pollfds.erase(pollIt);
	}
}

Channel* Server::create_channel(std::string const &name, std::string const &password)
{
	if (getChannel(name) != nullptr)
		return nullptr;
	Channel* newChannel = new Channel(name, password);
	_channels.push_back(newChannel);
	return newChannel;
}

bool Server::is_running()
{
    // Vérification si le socket du serveur est valide
    if (_fd_socket != -1)
    {
        // Vérification si le socket du serveur est en écoute
        struct pollfd pfd { _fd_socket, POLLIN, 0 };
        int pollResult = poll(&pfd, 1, 0);
        if (pollResult == -1)
        {
            perror("Erreur lors de l'appel à poll");
            return false;
        }
        // Retourne true si le socket du serveur est en écoute, sinon false
        return (pfd.revents & POLLIN);
    }
    return false;
}

bool Server::hasPassword()
{
    return !_password.empty();
}

const std::string Server::getPassword()
{
    return _password;
}

const std::string Server::getServerName()
{
    return _server_name;
}

std::vector<Channel*> Server::getChannels()
{
    return _channels;
}

Channel* Server::getChannel(std::string const &name)
{
    for (Channel* channel : _channels)
    {
        if (channel->getName() == name)
            return channel;
    }
    return nullptr;
}

std::vector<Client*> Server::getClients()
{
    std::vector<Client*> clients;
    for (const auto& clientPair : _clients)
        clients.push_back(clientPair.second);
    return clients;
}

Client* Server::getClient(std::string const &name)
{
    for (const auto& clientPair : _clients)
    {
        Client* client = clientPair.second;
        if (client->getNickname() == name)
            return client;
    }
    return nullptr;
}
