#include "render/html.h"
#include "util/file.h"

#include <al/malloc.h>

#include <string.h>
#include <stdio.h>
#include <malloc.h>

char *ews_render_html(const char *file)
{
	const char *header = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
	char *content;
	char *response;
	size_t len;

	content = ews_get_file_content(file);
	if (!content)
		return NULL;

	len = strlen(header) + strlen(content) + 1;
	response = al_malloc(len * sizeof(char));
	snprintf(response, len, "%s%s", header, content);

	free(content);
	return response;
}
