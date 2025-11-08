#include "Server.hpp"

// Example: PRIVMSG <destination> :<message>
void Server::handlePrivmsg(std::vector<std::string> &str_vtr, int index)
{
	if (!Clients[index]->get_bool_registered())
	{
		std::string msg_error = server_name + std::string(" 451 * : You have not registered\r\n");
		send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
		return ;
	}
	sender_index = index;
	if (str_vtr.size() < 3)
	{
		std::string msg_error = server_name + std::string(" 461 ") + Clients[index]->get_nickname() + std::string(" PRIVMSG : Not enough parameters\r\n");
		send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
		return ;
	}

	target = str_vtr[1];
	std::string message = str_vtr[2];
	if (!message.empty() && message[0] == ':')
		message = message.substr(1);

	msg2send = message;

	//! INICIO DA PARTE DO NANDO
	
	if(!target.empty() && target.find(',') != std::string::npos) // if i find ','
	{
		msg2send += "\r\n";
		std::vector<std::string> all_targets  = split_target(target);
		printf("targets: \n");
		for (std::vector<std::string>::iterator it = all_targets.begin(); it  != all_targets.end() ; it++)
			std::cout << "	"  << *it <<  std::endl;

		for (str_iterator it = all_targets.begin(); it != all_targets.end(); it++)
		{
			for (client_iterator client_it = Clients.begin(); client_it !=  Clients.end(); client_it++)
			{
				if (*it == (*client_it)->get_nickname())
				{
					std::cout << "client finded: " << *it << std::endl;
					std::cout << "msg2send: " << msg2send << std::endl;
					send((*client_it)->get_fd(), msg2send.c_str(), msg2send.size(), 0);
				}
			}			
			for ( channel_iterator channel_it = Channels.begin(); channel_it != Channels.end(); channel_it++)
			{
				if (*it == channel_it->get_name())
				{
					std::cout << "channel finded: " << *it << std::endl;
					int channel_index = distance(Channels.begin(), channel_it);
					std::cout <<  "channel_index: " << channel_index << std::endl; 
					broadcastToChannel(channel_index, msg2send.c_str(), Clients[index]);
				}
			}
		}
		return ;
	}
	//! FIM DA PARTE DO NANDO

	// If target is a channel (starts with '#'), broadcast to channel
	if (!target.empty() && target[0] == '#')
	{
		// Find channel index
		size_t chan_idx = Channels.size();
		for (size_t i = 0; i < Channels.size(); ++i)
		{
			if (Channels[i].get_name() == target)
			{
				chan_idx = i;
				break;
			}
		}
		if (chan_idx == Channels.size())
		{
			// No such channel
			std::string msg_error = server_name + std::string(" 403 ") + Clients[index]->get_nickname() + std::string(" ") + target + std::string(" :No such channel\r\n");
			send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
			return;
		}
		else
		{
			if (!Channels[chan_idx].hasClient(Clients[index]))
			{
				std::string msg_error = server_name + std::string(" 404 ") + Clients[index]->get_nickname() + std::string(" ") + target + std::string(" :Cannot send to channel\r\n");
				send(Clients[index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
				// Clean up
				msg2send.clear();
				target.clear();
				str_tokens.clear();
				return ;
			}
			if (msg2send.empty())
			{
				std::string msg_error = server_name + std::string(" 412 ") + Clients[sender_index]->get_nickname() + std::string(" PRIVMSG :No text to send\r\n");
				send(Clients[sender_index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);

			msg2send.clear();
			target.clear();
			str_tokens.clear();
			return ;
			}
			// Build the message and broadcast to channel (exclude sender)
			std::string privmsg = ":" + Clients[sender_index]->get_nickname() + "!" + Clients[sender_index]->get_username() + "@localhost PRIVMSG " + target + " :" + msg2send;
			broadcastToChannel(chan_idx, privmsg, Clients[sender_index]);
		}
		// Clean up
		msg2send.clear();
		target.clear();
		str_tokens.clear();
		return;
	}

	// Otherwise, target is a user nickname
	bool found = false;
	for (size_t i = 0; i < Clients.size(); i++)
	{
		if (target == Clients[i]->get_nickname())
		{
			found = true;
			if (msg2send.empty())
			{
				std::string msg_error = server_name + std::string(" 412 ") + Clients[sender_index]->get_nickname() + std::string(" PRIVMSG :No text to send\r\n");
				send(Clients[sender_index]->get_fd(), msg_error.c_str(), msg_error.size(), 0);
				msg2send.clear();
				target.clear();
				str_tokens.clear();
				return ;
			}
			std::string privmsg = ":" + Clients[sender_index]->get_nickname() + "!" + Clients[sender_index]->get_username() + "@localhost PRIVMSG " + target + " :" + msg2send + "\r\n";
			send(Clients[i]->get_fd(), privmsg.c_str(), privmsg.size(), 0);

			msg2send.clear();
			target.clear();
			str_tokens.clear();

			break ;
		}
	}
	if (!found)
	{
		std::string err = server_name + std::string(" 401 ") + Clients[sender_index]->get_nickname() + std::string(" ") + target + std::string(" :No such nick\r\n");
		send(Clients[sender_index]->get_fd(), err.c_str(), err.size(), 0);
	}
}