/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo <Leo@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 17:44:55 by Leo               #+#    #+#             */
/*   Updated: 2023/07/08 18:41:05 by Leo              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	printMsg(std::string const &msg)
{
	time_t		raw_time;
	struct tm	*time_info;
	char		buffer[80];

	time(&raw_time);
	time_info = localtime(&raw_time);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", time_info);
	std::string now(buffer);
	std::cout << "[" << now << "] " << msg << "\n";
}
