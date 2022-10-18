#include "server/http_server.h"
#include "render/html.h"
#include "server/http_request.h"
#include "server/config.h"

#include <al/malloc.h>
#include <clog/clog.h>
#include <al/string.h>
#include <al/map.h>

#include <malloc.h>
#include <unistd.h>
#include <string.h>

struct ews_http_client {
	int sockfd;
	struct ews_http_server *http;
};

static char *ews_http_get_uri_path(const char *uri)
{
	int flag = 0;
	if (!strcmp(uri, "/")) {
		uri = al_strdup("/index.html");
		flag = 1;
	}

	// NULL = PREV INITALIZED
	char *root_dir = ews_config_load(NULL, 0)->root;
	size_t len = strlen(root_dir) + strlen(uri) + 1;
	char *path = al_malloc(len);
	strncpy(path, root_dir, len - 1);
	strncat(path, uri, len - 1);

	if (flag)
		free(uri);

	return path;
}

static void *ews_http_handler(void *arg)
{
	struct ews_http_client *client = arg;
	struct ews_http_request *request = NULL;
	char *uripath = NULL;
	char *response = NULL;

	char *req = al_malloc(16385 * sizeof(char));
	int nread = read(client->sockfd, req, 16384);
	if (nread == -1 || nread == 0) {
		free(req);
		goto CLOSESOCKET;
	}

	request = ews_http_request_parse(req);
	free(req);

	char *uri = al_map_get(request->request_line, "uri");
	if (!uri)
		goto CLEANUP;

	uripath = ews_http_get_uri_path(uri);
	if (!uripath)
		goto CLEANUP;

	response = ews_render_html(uripath);
	if (!response)
		response = al_strdup(
			"HTTP/1.1 404 NOT FOUND\nContent-Type: text/html\n\n404 NOT FOUND");

	write(client->sockfd, response, strlen(response));
	free(response);

CLEANUP:
	ews_http_request_free(request);
	free(uripath);

CLOSESOCKET:
	shutdown(client->sockfd, SHUT_RDWR);
	close(client->sockfd);
	free(client);
	return NULL;
}

struct ews_http_server *ews_http_server_init(void)
{
	struct ews_http_server *http =
		al_malloc(sizeof(struct ews_http_server));
	http->server = NULL;
	http->threadpool = NULL;
	return http;
}

void ews_http_server_free(struct ews_http_server *http)
{
	if (http) {
		ews_threadpool_free(http->threadpool);
		ews_server_free(http->server);
		free(http);
	}
}

void ews_http_server_launch(struct ews_http_server *http,
			    struct ews_socket *sockconf, size_t nthreads)
{
	http->server = ews_server_init(sockconf);
	http->threadpool = ews_threadpool_init(nthreads);

	struct sockaddr *sa = (struct sockaddr *)http->server->sa;
	socklen_t addr_len = (socklen_t)sizeof(*http->server->sa);

	CLOG_INFO("RUNNING ON PORT:%d", http->server->socket->port);

	while (1) {
		struct ews_http_client *client =
			al_malloc(sizeof(struct ews_http_client));
		client->http = http;
		client->sockfd = accept(http->server->sockfd, sa, &addr_len);

		while (client->sockfd == -1)
			sleep(1);

		struct ews_threadtask *task =
			ews_threadpool_createtask(ews_http_handler, client);
		ews_threadpool_addtask(http->threadpool, task);
		free(task);
	}
}
