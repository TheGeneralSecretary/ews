#ifndef _SERVER_SERVER_H_
#define _SERVER_SERVER_H_

#include "socket.h"
#include <netinet/in.h>

/* EWS SERVER STRUCTURE
 * socket refers to socket configuration that was provided
 * sa is filled in with the address of the peer socket
 * sockfd is a file descriptor that was opened by socket
 */
struct ews_server {
	struct ews_socket *socket;
	struct sockaddr_in *sa;
	int sockfd;
};

struct ews_server *ews_server_init(struct ews_socket *sockconf);
void ews_server_free(struct ews_server *server);

#endif
