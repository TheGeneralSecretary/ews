// #include "server/server.h"
// #include "server/http_request.h"
// #include "server/http_server.h"
// #include "server/config.h"

// #include <clog/clog.h>
// #include <al/string.h>
// #include <al/malloc.h>

// #include <stdio.h>

// void tcp_listener(void)
// {
// 	struct ews_socket sock;
// 	sock.domain = AF_INET;
// 	sock.type = SOCK_STREAM;
// 	sock.protocol = 0;
// 	sock.host = INADDR_ANY;
// 	sock.port = 3378;
// 	sock.backlog = 255;

// 	struct ews_server *tcp = ews_server_init(&sock);
// 	if (!tcp)
// 		CLOG_FATAL("FAILED TO INIT SERVER");

// 	printf("SOCKFD: %d\n", tcp->sockfd);

// 	ews_server_free(tcp);
// }

// void http_request(void)
// {
// 	const char *httpreq =
// 		"POST / HTTP/1.1\nHost: 0.0.0.0\nUser-Agent: nodejs/2.25.1\nAccept-Encoding: gzip, deflate\nAccept: */*\nConnection: keep-alive\nContent-Length: 16\nContent-Type: application/x-www-form-urlencoded\n\nmax=2&jacky=rexi";
// 	struct ews_http_request *request = ews_http_request_parse(httpreq);
// 	if (!request)
// 		CLOG_FATAL("FAILED TO PARSE REQUEST");

// 	// REQUEST LINE
// 	printf("METHOD: %s\n",
// 	       (char *)al_map_get(request->request_line, "method"));
// 	printf("URI: %s\n", (char *)al_map_get(request->request_line, "uri"));
// 	printf("HTTP VERSION: %s\n",
// 	       (char *)al_map_get(request->request_line, "http_version"));

// 	// HEADERS
// 	printf("HOST: %s\n",
// 	       (char *)al_map_get(request->header_fields, "Host"));
// 	printf("User-Agent: %s\n",
// 	       (char *)al_map_get(request->header_fields, "User-Agent"));
// 	printf("Accept-Encoding: %s\n",
// 	       (char *)al_map_get(request->header_fields, "Accept-Encoding"));

// 	// BODY
// 	printf("BODY: %s\n", (char *)al_map_get(request->body, "max"));
// 	printf("BODY: %s\n", (char *)al_map_get(request->body, "jacky"));

// 	ews_http_request_free(request);
// }

// void http_server(void)
// {
// 	struct ews_http_server *http = ews_http_server_init();
// 	if (!http)
// 		CLOG_FATAL("FAILED TO INIT HTTP SERVER");

// 	struct ews_socket sock;
// 	sock.domain = AF_INET;
// 	sock.type = SOCK_STREAM;
// 	sock.protocol = 0;
// 	sock.host = INADDR_ANY;
// 	sock.port = 3378;
// 	sock.backlog = 255;

// 	ews_http_server_launch(http, &sock, 12);
// }

// void test_config(void)
// {
// 	/* Create Config */
// 	struct ews_config *conf = al_malloc(sizeof(struct ews_config));
// 	conf->host = al_strdup("0.0.0.0");
// 	conf->port = al_strdup("3333");
// 	conf->root = al_strdup("/var/www/html");

// 	int s = ews_config_create(conf, "/etc/ews/config.ini");
// 	if (s == -1)
// 		CLOG_FATAL("FAILED TO INIT CONFIG");

// 	ews_config_free(conf);

// 	/* Load Config */
// 	struct ews_config *config = ews_config_load("/etc/ews/config.ini", 0);
// 	if (!config)
// 		CLOG_FATAL("FAILED TO LOAD CONFIG");

// 	printf("HOST: %s\nPORT:%s\nROOT:%s\n", config->host, config->port,
// 	       config->root);

// 	ews_config_free(config);
// }
