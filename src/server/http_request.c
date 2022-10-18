#include "server/http_request.h"
#include "server/http_parser.h"

#include <al/malloc.h>
#include <al/string.h>

#define _GNU_SOURCE
#include <string.h>
#include <malloc.h>

struct ews_http_request *ews_http_request_parse(const char *request)
{
	struct ews_http_request *req =
		al_malloc(sizeof(struct ews_http_request));
	req->request_line = NULL;
	req->header_fields = NULL;
	req->body = NULL;

	char *saveptr = NULL;
	char *reqstr = al_strdup(request);

	for (size_t i = 0; i < strlen(reqstr) - 1; i++)
		if (reqstr[i] == '\n' && reqstr[i + 1] == '\n')
			reqstr[i + 1] = '|';

	char *request_line = strtok_r(reqstr, "\n", &saveptr);
	char *header_fields = strtok_r(NULL, "|", &saveptr);
	char *body = strtok_r(NULL, "|", &saveptr);

	if (request_line)
		ews_http_parse_request_line(req, request_line);

	if (header_fields)
		ews_http_parse_header_fields(req, header_fields);

	if (body)
		ews_http_parse_body(req, body);

	free(reqstr);
	return req;
}

void ews_http_request_free(struct ews_http_request *request)
{
	if (request) {
		al_map_free(request->request_line);
		al_map_free(request->header_fields);
		al_map_free(request->body);
		free(request);
	}
}
