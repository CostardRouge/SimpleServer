#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
	printf(PROJECT" Server shutting down...\n");
	close(this->network_info.socket);
}

bool Server::InitServer(unsigned short port)
{
	t_network_contener		*ret;

	ret = &this->network_info;
	//ret->name = strdup((name != NULL ? name : getenv("USER")));
	ret->pe = getprotobyname("TCP");
	if ((ret->socket = socket(AF_INET, SOCK_STREAM, ret->pe->p_proto)) == -1)
		return (false);
	  //exit_msg(1, "SERVER: Socket failed. :(");
	ret->sin.sin_family = AF_INET;
	ret->sin.sin_port = htons(port);
	ret->sin.sin_addr.s_addr = INADDR_ANY;
	if ((bind(ret->socket, (const struct sockaddr *)&ret->sin, sizeof(ret->sin))) == -1)
		return (false);
	  //exit_close_msg(1, "SERVER: Bind failed. :(", ret);
	if ((listen(ret->socket, 32)) == -1)
		return (false);
	  //exit_close_msg(1, "SERVER: Listen failed. :(", ret);
	printf(PROJECT" SERVER: Waiting for a client connection. :)\n");
	return (true);
}

void 			Server::SelectConnection()
{
	int			range;

    this->network_info.tv.tv_sec = 1;
    this->network_info.tv.tv_usec = 0;
    FD_ZERO(&this->network_info.readf);
    FD_SET(this->network_info.socket, &this->network_info.readf);

	std::list<unsigned int>::iterator it = this->clients.begin();
	while (it != this->clients.end()) {
		if ((*it) != -1)
			FD_SET((*it), &this->network_info.readf);
		++it;
	}

	range = this->network_info.socket + 1 + this->clients.size();
    if (select(range, &this->network_info.readf, NULL, NULL, &this->network_info.tv) == -1)
		return;  //exit_close_msg(1, "SERVER: Select failed. :(", irc);
}

void 			Server::ClientConnection()
{
	int			client_socket;
    //char		name[80];
    socklen_t   len;
    static int	id;

    len = sizeof(this->network_info.cli);
    if (FD_ISSET(this->network_info.socket, &this->network_info.readf))
      {
        if ((client_socket = accept(this->network_info.socket, (struct sockaddr *)&this->network_info.cli, &len)) == -1)
			return; //exit_close_msg(1, "SERVER: Accept failed. :(", irc);
        printf(PROJECT" Client connected, IP: %s. :)\n", inet_ntoa(this->network_info.cli.sin_addr));
        //sprintf(name, "User_%d", id);
		this->clients.push_front(client_socket);
        id++;
      }
}

void 			Server::HandleConnection()
{
	int		read_result;
    char	receive_stream[BUFSIZE];

	std::list<unsigned int>::iterator it = this->clients.begin();
	while (it != this->clients.end())
	{
		if ((*it != -1) && FD_ISSET(*it, &this->network_info.readf))
		{
			read_result = read(*it, receive_stream, BUFSIZE);
			if (read_result <= 0)
			{
				printf(PROJECT" SERVER: An user die. :(\n");
				if (close(*it) == -1)
		  			return; //exit_msg(1, "Close failed. :(");
				*it = -2; // Our boy is now disconnected
		    }
			else {
				receive_stream[read_result] = '\0';
				std::string msg = "USER_" + std::to_string(*it) + " : " + receive_stream;

				BroadcastStringToEveryone(msg.c_str());
			}
		}
		++it;
	}
}

void Server::BroadcastStringToEveryone(const std::string & message)
{
	int		write_result;

	std::list<unsigned int>::iterator it = this->clients.begin();
	while (it != this->clients.end())
	{
		if ((*it) != -1)
		{
			write_result = write((*it) , message.c_str(), message.length());
		}
		++it;
	}
}

void Server::StartServing()
{
	while (true) {
		SelectConnection();
		ClientConnection();
		HandleConnection();
	}
}

bool Server::Run()
{
	if (!InitServer(4000))
	{
		printf("Failed to init server.\n");
		return (false);
	}
	StartServing();
	return (true);
}
