#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

struct ews_config {
	const char *host;
	const char *port;
	const char *root;
};

int ews_config_create(struct ews_config *config, const char *confpath);
struct ews_config *ews_config_load(const char *confpath, int reload);
void ews_config_free(struct ews_config *config);

#endif
