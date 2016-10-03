#ifndef		SERVER_HPP_
# define	SERVER_HPP_

# include	<sys/socket.h>
# include	<netinet/in.h>
# include	<arpa/inet.h>
# include	<sys/types.h>
# include	<sys/time.h>
# include	<sys/stat.h>
# include	<strings.h>
# include	<dirent.h>
# include	<string.h>
# include	<stdlib.h>
# include	<unistd.h>
# include	<signal.h>
# include	<netdb.h>
# include	<stdio.h>
# include	<time.h>

# include	<list>
# include	<string>

# define	BUFSIZE	1024
# define	PROJECT	"[MY NOOB PROJECT]"
# define	SUCCESS	"[SUCCESS] Executed. :)"
# define	WELCOME	"[SUCCESS] Welcome. :)"

typedef struct sockaddr		t_addr;
typedef struct protoent		t_protoent;
typedef struct timeval		t_timeval;
typedef struct sockaddr_in	t_s_addr_in;

typedef struct	s_network_contener
{
	unsigned short 	port;
	unsigned int 	socket;
	char 			ip[32];

	t_protoent		*pe;
	t_s_addr_in		sin;
	t_s_addr_in		cli;
	t_timeval		tv;
	fd_set			readf;
	fd_set			writef;

}	t_network_contener;

class Server
{
public:
	Server();
	~Server();

	bool 			InitServer(unsigned short port);
	void 			StartServing();
	void 			EndServing();

	void 			SelectConnection();
	void 			ClientConnection();
	void 			HandleConnection();

	void 			BroadcastStringToEveryone(const std::string & string);
	bool 			Run();

private:
	t_network_contener		network_info;
	std::list<unsigned int>	clients;
};

#endif
