#include "Channels.hpp"
#include "Server.hpp"

void Channel::handleKeyMode(std::string key)
{
	if (key.empty())
	{
		std::cout << RED << "[ERROR]" << PINK << "[MODE]" << RESET << " Channel key cannot be empty" << std::endl;
		return ;
	}
	
	if (key.size() > 20)
	{
		//! VER QUAL UM NUMERO ACEITAVEL DE CARACTERES
		std::cout << RED << "[ERROR]" << PINK << "[MODE]" << RESET << " Channel key too long, maximum is 20 characters" << std::endl;
		return ;
	}

	for (size_t i = 0; i < key.size(); i++)
	{
		unsigned char c = static_cast<unsigned char>(key[i]);

		// 2. Cannot contain spaces, commas, or control G (ASCII 7)
		if (c == 0x00 || c == 0x0A || c == 0x0D || c == 0x20 || c == 0x40)
		{
			std::cout << RED << "[ERROR]" << PINK << "[MODE]" << RESET << " Channel key contains invalid characters" << std::endl;
			return ;
		}
	}

	addMode('k');  // Use the string-based mode system
	channel_key = key;

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "Channel key for channel " << name << " set to " << channel_key << std::endl;
	}
}

void Channel::removeKey()
{
	removeMode('k');  // Use the string-based mode system
	channel_key = "";

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "Channel key for channel " << name << " removed" << std::endl;
	}
}

void Channel::handleTopicMode()
{
	addMode('t');  // Use the string-based mode system

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "Channel topic for channel " << name << " set to be changed by operators only" << std::endl;
	}
}

void Channel::removeTopicMode()
{
	removeMode('t');  // Use the string-based mode system

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "Channel topic for channel " << name << " can now be changed by anyone" << std::endl;
	}
}

void Channel::handleInviteMode()
{
	addMode('i');  // Use the string-based mode system

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "Invite only mode set for channel " << name << std::endl;
	}
}

void Channel::removeInviteMode()
{
	removeMode('i');  // Use the string-based mode system

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "Invite only mode removed for channel " << name << std::endl;
	}
}

void Channel::handleLimitMode(int limit)
{
	if (limit <= 0)
	{
		std::cout << RED << "[ERROR]" << PINK << "[MODE]" << RESET << " User limit must be positive" << std::endl;
		return ;
	}

	if (limit > 999999)
	{
		std::cout << RED << "[ERROR]" << PINK << "[MODE]" << RESET << " User limit too high, maximum is 999999" << std::endl;
		return ;
	}

	addMode('l');  // Use the string-based mode system
	user_limit = limit;

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "User limit set to " << limit << " for channel " << name << std::endl;
	}
}

void Channel::removeLimitMode()
{
	removeMode('l');  // Use the string-based mode system
	user_limit = -1;  // -1 means no limit

	if (DEBUG)
	{
		std::cout << GREEN << "[SUCCESS]" << RESET << "User limit removed for channel " << name << std::endl;
	}
}
