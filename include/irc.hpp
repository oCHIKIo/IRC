//badreddine

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <sstream>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <cerrno>
# include <csignal>
# include <cctype>
# include <algorithm>

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include <netdb.h>

# define MAX_CLIENTS  128
# define BUFFER_SIZE  512

extern bool g_running;

#endif
