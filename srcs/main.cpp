/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-jesu <fde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:05:00 by afogonca          #+#    #+#             */
/*   Updated: 2025/08/21 14:42:29 by fde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"
#include "../incs/Server.hpp"

Server *data;

void signalHandler(int signum) {
  if (signum == SIGPIPE || signum == SIGSEGV)
    return;
  if (data) {
    delete data;
    data = NULL;
  }
  _exit(signum);
}
// Implement size limit of 512 including \r\n, following
// the RFC 1459 protocol
int main(int ac, char **av) {
  if (ac != 3) {
    std::cout << "Error\nInvalid Number of Arguments!" << std::endl;
    return (1);
  }
  signal(SIGINT, &signalHandler);
  data = new Server(std::atoi(av[1]), av[2]);
  if (data->getPort() == -1) { // no constructor temos isto na  
	return (1);// lista de inicializacao, entao so sera -1 se o  segundo argumento for literamente '-1'
  }
  std::cout << "Server listening on port " << data->getPort() << std::endl;
  data->serverListen();
  (void)av;
  return 0;
}
