//badreddine

#include "Server.hpp"

bool g_running = true;

static void signalHandler(int sig)
{
	(void)sig;
	g_running = false;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = std::atoi(argv[1]);
	if (port <= 0 || port > 65535) {
		std::cerr << "Error: invalid port" << std::endl;
		return 1;
	}
	std::string password = argv[2];
	if (password.empty()) {
		std::cerr << "Error: password cannot be empty" << std::endl;
		return 1;
	}

	signal(SIGINT, signalHandler);
	signal(SIGPIPE, SIG_IGN);

	try {
		Server server(port, password);
		server.run();
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

/*	
	--> a socket is a software object and a communication endpoint 
		socket associated with ip adress + port + protocol (udp/tcp)
	
	--> socket() this is a system call provided by the OS
		its job to create an empy endpoint communication (a socket object)
		and return a file descriptor
*/
