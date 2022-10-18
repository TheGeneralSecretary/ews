#ifndef _SERVER_HTTP_SERVER_H_
#define _SERVER_HTTP_SERVER_H_

#include "server.h"
#include "threadpool.h"

/* HTTP SERVER STRUCTURE
 * server holds socket configurations
 * threadpool holds the threadpool
 */
struct ews_http_server {
	struct ews_server *server;
	struct ews_threadpool *threadpool;
};

struct ews_http_server *ews_http_server_init(void);
void ews_http_server_free(struct ews_http_server *http);
void ews_http_server_launch(struct ews_http_server *http,
			    struct ews_socket *sockconf, size_t nthreads);

#endif
