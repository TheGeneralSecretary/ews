#ifndef _SERVER_HTTP_REQUEST_H_
#define _SERVER_HTTP_REQUEST_H_

#include <al/map.h>

struct ews_http_request {
	struct al_map *request_line;
	struct al_map *header_fields;
	struct al_map *body;
};

struct ews_http_request *ews_http_request_parse(const char *request);
void ews_http_request_free(struct ews_http_request *request);

#endif
