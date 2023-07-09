/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:58:01 by Leo               #+#    #+#             */
/*   Updated: 2023/07/09 17:31:29 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

CommandHandler::CommandHandler(Server *server)
    : _server(server)
{
    _commands["PASS"] = new PassCommand(server, 0);
    _commands["NICK"] = new NickCommand(server, 0);
    _commands["USER"] = new UserCommand(server, 0);
    _commands["QUIT"] = new QuitCommand(server, 0);

    _commands["PING"] = new PingCommand(server, 1);
    _commands["PONG"] = new PongCommand(server, 1);
    _commands["JOIN"] = new JoinCommand(server, 1);
    _commands["PART"] = new PartCommand(server, 1);
    _commands["PRIVMSG"] = new PrivMsgCommand(server, 1);
    _commands["NOTICE"] = new NoticeCommand(server, 1);

    _commands["KICK"] = new KickCommand(server, 2);
}

CommandHandler::~CommandHandler()
{
    // Libérer la mémoire des commandes
    for (auto& pair : _commands)
    {
        delete pair.second;
    }
}

int CommandHandler::handle_command(Client *client, std::string &line)
{
    // Extraire le nom de la commande et les arguments
    std::istringstream iss(line);
    std::string command_name;
    iss >> command_name;
    
    // Vérifier si la commande existe dans la map
    auto it = _commands.find(command_name);
    if (it != _commands.end())
    {
        Command* command = it->second;
        
        // Vérifier les autorisations d'exécution de la commande
        if (command->getIsAuth() <= client->getStatus())
        {
            std::vector<std::string> args;
            std::string arg;
            while (iss >> arg)
            {
                args.push_back(arg);
            }
            
            // Exécuter la commande
            command->execute(client, args);
            
            return 1; // La commande a été traitée avec succès
        }
        else
        {
            // Autorisations insuffisantes
            client->sendReply(ERR_NOTREGISTERED(client->getNickname()));
        }
    }
    else
    {
        // Commande inconnue
        client->sendReply(ERR_UNKNOWNCOMMAND(client->getNickname(), command_name));
    }
    
    return 0; // La commande n'a pas été traitée
}
