#include "server/config.h"

#include <clog/clog.h>
#include <al/string.h>
#include <al/malloc.h>
#include <ini.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>

static struct ews_config *configuration = NULL;

static int ews_config_handler(void *server, const char *section,
			      const char *key, const char *value)
{
	struct ews_config *config = server;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(key, n) == 0
	if (MATCH("server", "host")) {
		config->host = al_strdup(value);
	} else if (MATCH("server", "port")) {
		config->port = al_strdup(value);
	} else if (MATCH("server", "root")) {
		config->root = al_strdup(value);
	} else {
		return 0; /* Invalid section/name */
	}

	return 1;
}

int ews_config_create(struct ews_config *config, const char *confpath)
{
	if (confpath[0] != '/')
		return -1;

	char *path = al_strdup(confpath);
	path[(strrchr(confpath, '/') - confpath)] = '\0';

	mkdir(path, 0755);

	if (access(path, F_OK) != 0) {
		CLOG_ERROR("COULD NOT CREATE CONFIG DIR\nNO PERMISSION");
		return -1;
	}

	int fd = open(confpath, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1) {
		CLOG_ERROR("COULD NOT CREATE CONFIG FILE\nNO PERMISSION");
		return -1;
	}

	size_t conf_len = strlen(config->host) + strlen(config->port) +
			  strlen(config->root) + strlen("host=") +
			  strlen("port=") + strlen("root=") +
			  strlen("[server]") + 5;

	char *conf = al_malloc(conf_len);

	int write_len = snprintf(conf, conf_len,
				 "[server]\nhost=%s\nport=%s\nroot=%s\n",
				 config->host, config->port, config->root);

	if (write_len < 0 || write_len >= conf_len) {
		CLOG_ERROR("FAILED TO WRITE CONFIG");
		return -1;
	}

	int ret = write(fd, conf, strlen(conf));
	close(fd);
	free(conf);
	free(path);
	return ret;
}

struct ews_config *ews_config_load(const char *confpath, int reload)
{
	if (configuration && !reload)
		return configuration;

	if (configuration) {
		free(configuration->host);
		free(configuration->port);
		free(configuration->root);
		free(configuration);
	}

	configuration = al_malloc(sizeof(struct ews_config));

	if (ini_parse(confpath, ews_config_handler, configuration) < 0)
		return NULL;

	return configuration;
}

void ews_config_free(struct ews_config *config)
{
	if (config) {
		free(config->host);
		free(config->port);
		free(config->root);
		free(config);
	}
}
