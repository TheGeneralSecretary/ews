#include "server/http_parser.h"

#include <al/string.h>
#include <al/queue.h>

#define _GNU_SOURCE
#include <string.h>
#include <malloc.h>

void ews_http_parse_request_line(struct ews_http_request *request,
				 const char *request_line)
{
	char *reqline = al_strdup(request_line);
	char *saveptr = NULL;

	char *method = strtok_r(reqline, " ", &saveptr);
	char *uri = strtok_r(NULL, " ", &saveptr);
	char *http_version = strtok_r(NULL, "", &saveptr);

	request->request_line = al_map_init();
	al_map_insert(request->request_line, "method", method,
		      strlen(method) + 1);
	al_map_insert(request->request_line, "uri", uri, strlen(uri) + 1);
	al_map_insert(request->request_line, "http_version", http_version,
		      strlen(http_version) + 1);

	free(reqline);
}

void ews_http_parse_header_fields(struct ews_http_request *request,
				  const char *header_fields)
{
	char *headerfields = al_strdup(header_fields);
	char *saveptr = NULL;
	struct al_queue *headers = al_queue_init();

	/* Enqueue all header fields */
	char *field = strtok_r(headerfields, "\n", &saveptr);
	while (field) {
		al_queue_push(headers, field, strlen(field) + 1);
		field = strtok_r(NULL, "\n", &saveptr);
	}
	saveptr = NULL;

	/* Insert header fields in map */
	request->header_fields = al_map_init();

	char *header = (char *)al_queue_pop(headers);
	while (header) {
		char *key = strtok_r(header, ":", &saveptr);
		char *value = strtok_r(NULL, "", &saveptr);

		if (value) {
			if (value[0] == ' ')
				value++;

			al_map_insert(request->header_fields, key, value,
				      strlen(value) + 1);
		}

		free(header);
		header = al_queue_pop(headers);
	}

	al_queue_free(headers);
	free(headerfields);
}

void ews_http_parse_body(struct ews_http_request *request, const char *body)
{
	char *reqbody = al_strdup(body);
	char *saveptr = NULL;
	char *content_type = NULL;

	/* Extract Content-Type from header fields */
	if (request->header_fields)
		content_type =
			al_map_get(request->header_fields, "Content-Type");

	/* Parse Body */
	if (content_type) {
		request->body = al_map_init();

		/* application/x-www-form-urlencoded parser */
		if (!strcmp(content_type,
			    "application/x-www-form-urlencoded")) {
			/* Enqueue all fields */
			struct al_queue *fields = al_queue_init();

			char *field = strtok_r(reqbody, "&", &saveptr);
			while (field) {
				al_queue_push(fields, field, strlen(field) + 1);
				field = strtok_r(NULL, "&", &saveptr);
			}
			saveptr = NULL;

			/* Parse fields */
			field = al_queue_pop(fields);
			while (field) {
				char *key = strtok_r(field, "=", &saveptr);
				char *value = strtok_r(NULL, "", &saveptr);

				if (value[0] == ' ')
					value++;

				al_map_insert(request->body, key, value,
					      strlen(value) + 1);

				free(field);
				field = al_queue_pop(fields);
			}

			al_queue_free(fields);
		} else {
			/* If unknown content type, just store the data */
			al_map_insert(request->body, "data", reqbody,
				      strlen(reqbody) + 1);
		}
	}

	free(reqbody);
}
