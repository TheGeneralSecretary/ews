#include "handler.h"
#include "ews/ews.h"
#include "server/config.h"
#include "server/http_server.h"
#include "server/socket.h"

#include <clog/clog.h>

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

struct ews_http_server *http_server = NULL;
struct ews_config *config = NULL;

int command_handler(int argc, char **argv)
{
	if (argc <= 1) {
		puts("usage: ews [--version] [--help] [--init] [--start] <args>");
		return 0;
	}

	if (!strncmp("--version", argv[1], strlen("--version"))) {
		CLOG_INFO("VERSION %d", EWS_VERSION);
		return 0;
	}

	if (!strncmp("--help", argv[1], strlen("--help"))) {
		puts("usage: ews [--version] [--help] [--start] <args>");
		puts("EWS is a HTTP Web Server for Unix-Like Operating Systems");
		puts("Author: General Secretary");
		puts("License: GPL-3");
		puts("Project: https://github.com/TheGeneralSecretary/ews");
		return 0;
	}

	/* Initialize configuration file */
	if (!strncmp("--init", argv[1], strlen("--init"))) {
		if (argc < 5) {
			puts("usage: sudo ews --init <HOST> <PORT> <ROOTDIR>");
			return 1;
		}

		/* Extract host and port */
		struct ews_config conf = { argv[2], argv[3], argv[4] };
		if (ews_config_create(&conf, "/etc/ews/config.ini") < 0)
			return 1;

		CLOG_INFO("INITIALIZED");
		return 0;
	}

	if (!strncmp("--start", argv[1], strlen("--start"))) {
		if (argc < 3) {
			puts("usage: ews --start <NTHREADS>");
			return 1;
		}

		int nthreads = atoi(argv[2]);

		config = ews_config_load("/etc/ews/config.ini", 1);
		if (!config) {
			CLOG_ERROR(
				"FAILED TO LOAD CONFIG\nPLEASE RUN `--init`");
			return 1;
		}

		struct ews_socket sock = { AF_INET,
					   SOCK_STREAM,
					   0,
					   inet_addr(config->host),
					   atoi(config->port),
					   255 };

		CLOG_INFO("STARTING EWS WITH %d THREADS...", nthreads);
		http_server = ews_http_server_init();
		ews_http_server_launch(http_server, &sock, nthreads);
		ews_http_server_free(http_server);

		return 0;
	}

	puts("usage: ews [--version] [--help] [--init] [--start] <args>");
	return 0;
}
