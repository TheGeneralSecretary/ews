#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

#include <sys/socket.h>

/* EWS Socket Structure
 * domain specifies a communication domain
 * type specifies the communication semantics
 * protocol specifies a particular protocol to be used with the socket. TCP/UDP
 * host specifies the host address
 * port specifies the port number
 * backlog defines the maximum length to which the queue of pending connections for socket may grow.
 */
struct ews_socket {
	int domain;
	int type;
	int protocol;
	int host;
	int port;
	int backlog;
};

#endif
