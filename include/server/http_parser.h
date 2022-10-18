#ifndef _SERVER_HTTP_PARSER_H_
#define _SERVER_HTTP_PARSER_H_

#include "http_request.h"

void ews_http_parse_request_line(struct ews_http_request *request,
				 const char *request_line);
void ews_http_parse_header_fields(struct ews_http_request *request,
				  const char *header_fields);
void ews_http_parse_body(struct ews_http_request *request, const char *body);

#endif
