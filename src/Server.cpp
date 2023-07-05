/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:57:36 by Leo               #+#    #+#             */
/*   Updated: 2023/07/05 19:09:19 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Server::Server(std::string const &port, std::string const &password) :
	_port(port),
	_password(password),
	_fd_socket(this->create_socket()),
	_running(1),
	_server_name(DEFAULT_SERVER_NAME),
	_server_time(dateString()),
	_command_handler(new CommandHandler(this))
{
	// Initialise les membres de la classe
}

Server::~Server()
{
	// Effectuez la libération de mémoire ou la fermeture des ressources si nécessaire
}

void Server::run()
{
    // Ajout du socket du serveur dans le vecteur des pollfds
	pollfd	server_fd = {this->_fd_socket, POLLIN, 0};
    _pollfds.push_back(server_fd);

    // Boucle principale du serveur
    while (is_running())
    {
        // Attente des événements sur les sockets avec la fonction poll()
        int pollResult = poll(_pollfds.data(), _pollfds.size(), -1);
        if (pollResult == -1)
        {
            // Gestion de l'erreur lors de l'appel à poll()
            perror("Erreur lors de l'appel à poll");
            close_server();
            exit(EXIT_FAILURE);
        }

        // Vérification des événements sur les sockets
        for (size_t i = 0; i < _pollfds.size(); ++i)
        {
            if (_pollfds[i].revents & POLLIN)
            {
                if (_pollfds[i].fd == sockfd)
                {
                    // Nouvelle connexion entrante sur le socket du serveur
                    int clientSockfd = accept(sockfd, nullptr, nullptr);
                    if (clientSockfd == -1)
                    {
                        // Gestion de l'erreur lors de l'acceptation de la connexion
                        perror("Erreur lors de l'acceptation de la connexion");
                        continue;
                    }

                    // Création d'un nouveau client et ajout dans la map des clients
                    Client* newClient = new Client(clientSockfd);
                    _clients[clientSockfd] = newClient;

                    // Ajout du socket du client dans le vecteur des pollfds
                    _pollfds.push_back({ clientSockfd, POLLIN, 0 });

                    // Envoi du message de bienvenue au nouveau client
                    newClient->sendWelcomeMessage();
                }
                else
                {
                    // Données disponibles à lire sur un socket client
                    Client* client = _clients[_pollfds[i].fd];
                    client->receiveData();
                }
            }
            else if (_pollfds[i].revents & POLLHUP)
            {
                // Fermeture de la connexion sur un socket client
                Client* client = _clients[_pollfds[i].fd];
                client->closeConnection();

                // Suppression du socket client des pollfds et de la map des clients
                _pollfds.erase(_pollfds.begin() + i);
                _clients.erase(_pollfds[i].fd);

                // Décrémentation de l'index pour éviter les décalages
                --i;
            }
        }
    }
}

void Server::close_server()
{
	// Code pour fermer le serveur IRC
}

int Server::create_socket()
{
    // Création du socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        // Gestion de l'erreur lors de la création du socket
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse et du port du serveur
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(_port)); // Conversion du port en entier
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Liaison du socket à l'adresse et au port du serveur
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        // Gestion de l'erreur lors de la liaison du socket
        perror("Erreur lors de la liaison du socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Mise en écoute du socket
    if (listen(sockfd, SOMAXCONN) == -1)
    {
        // Gestion de l'erreur lors de la mise en écoute du socket
        perror("Erreur lors de la mise en écoute du socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

	// Retourne le descripteur de fichier du socket cré
    return sockfd;
}

Channel* Server::create_channel(std::string const &name, std::string const &password)
{
	// Code pour créer un nouveau canal avec le nom et le mot de passe spécifiés
	// Retourne un pointeur vers le canal créé
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
            // Gestion de l'erreur lors de l'appel à poll()
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
	// Code pour vérifier si le serveur a un mot de passe défini
	// Retournez true si un mot de passe est défini, sinon false
}

const std::string Server::getPassword()
{
	// Code pour obtenir le mot de passe du serveur
	// Retournez le mot de passe du serveur
}

const std::string Server::getServerName()
{
	// Code pour obtenir le nom du serveur
	// Retournez le nom du serveur
}

std::vector<Channel*> Server::getChannels()
{
	// Code pour obtenir la liste des canaux du serveur
	// Retournez un vecteur contenant des pointeurs vers les canaux
}

Channel* Server::getChannel(std::string const &name)
{
	// Code pour obtenir un canal spécifique par son nom
	// Retournez un pointeur vers le canal correspondant au nom spécifié
}

std::vector<Client*> Server::getClients()
{
	// Code pour obtenir la liste des clients connectés au serveur
	// Retournez un vecteur contenant des pointeurs vers les clients
}

Client* Server::getClient(std::string const &name)
{
	// Code pour obtenir un client spécifique par son nom
	// Retournez un pointeur vers le client correspondant au nom spécifié
}
