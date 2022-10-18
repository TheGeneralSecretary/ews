#include "server/server.h"

#include <al/malloc.h>

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

struct ews_server *ews_server_init(struct ews_socket *sockconf)
{
	struct ews_server *server = al_malloc(sizeof(struct ews_server));

	server->socket = al_malloc(sizeof(struct ews_socket));
	memcpy(server->socket, sockconf, sizeof(struct ews_socket));

	server->sa = al_malloc(sizeof(struct sockaddr_in));
	server->sa->sin_family = sockconf->domain;
	server->sa->sin_port = htons(sockconf->port);
	server->sa->sin_addr.s_addr = htonl(sockconf->host);

	server->sockfd =
		socket(sockconf->domain, sockconf->type, sockconf->protocol);
	if (server->sockfd == -1) {
		perror("socket");
		return NULL;
	}

	int opt = 1;
	if (setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &opt,
		       sizeof(opt)) == -1) {
		perror("setsockopt");
		return NULL;
	}

	if (bind(server->sockfd, (struct sockaddr *)server->sa,
		 sizeof(*server->sa)) < 0) {
		perror("bind");
		return NULL;
	}

	if (listen(server->sockfd, sockconf->backlog) == -1) {
		perror("listener");
		return NULL;
	}

	return server;
}

void ews_server_free(struct ews_server *server)
{
	if (server) {
		free(server->socket);
		free(server->sa);
		close(server->sockfd);
		free(server);
	}
}
