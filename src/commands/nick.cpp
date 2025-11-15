#include "Server.hpp"

void Server::handleNick(std::vector<std::string> &tokens, int index)
{
	if (!Clients[index]->get_bool_password_implemented())
	{
		std::string msg_error = ":" + server_name + " 464 * :Password required\r\n"; // CHANGE
		send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
		return ;
	}
	if (tokens.size() < 2)
	{
	std::string msg_error = server_name + std::string(" 431 * : No nickname given\r\n");
	send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
		return ;
	}
	std::string nick = tokens[1];

	if (nick.empty())
	{
	std::string msg_error = server_name + std::string(" 431 * : No nickname given\r\n");
	send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
		return ;
	}

	if (nick.length() > 9)
	{
		std::string msg_error = server_name + std::string(" 432 * ") + nick + std::string(": Erroneous nickname\r\n");
		send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
		return ;
	}

	for (size_t i = 0; i < Clients.size(); i++)
	{
		if (Clients[i]->get_nickname() == nick)
		{
			std::string msg_error = server_name + std::string(" 433 ") + nick + std::string(" : Nickname is already in use\r\n");
			send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
			return ;
		}
	}

	std::string invalid_chars = " ,*#@?!@.";
	std::string invalid_start_chars = "$:#&+%~-";

	for (size_t i = 0; i < invalid_chars.size(); i++)
	{
		if (nick.find(invalid_chars[i]) != std::string::npos)
		{
		std::string msg_error = server_name + std::string(" 432 * ") + nick + std::string(": Erroneous nickname\r\n"); 
		send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
			return ;
		}
	}

	for (size_t i = 0; i < invalid_start_chars.size(); i++)
	{
		if (nick[0] == invalid_start_chars[i])
		{
			std::string msg_error = server_name + std::string(" 433 ") + nick + std::string(" : Nickname is already in use\r\n");
			send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
			return ;
		}
 	}

	Clients[index]->set_nickname(nick);
	if (!Clients[index]->get_nickname().empty() && !Clients[index]->get_username().empty())
		Clients[index]->set_bool_registered(true);

	if (DEBUG || EVAL)
		std::cout << GREEN << "[SUCCESS]" << PINK << "[NICK]" << RESET << " Nickname set to " << nick << std::endl;
}
