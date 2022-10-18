#define _XOPEN_SOURCE 700

#include "ews/ews.h"
#include "handler.h"
#include "server/http_server.h"
#include "server/config.h"

#include <clog/clog.h>

#include <signal.h>
#include <stdlib.h>

extern struct ews_http_server *http_server;
extern struct ews_config *config;

void signal_handler(int signum)
{
	printf("SIGNAL: %d\n", signum);
	ews_http_server_free(http_server);
	ews_config_free(config);
	exit(0);
}

void init_signal_handler(void)
{
	struct sigaction sigact, sigactold;

	sigact.sa_handler = signal_handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;

	sigaction(SIGINT, NULL, &sigactold);
	if (sigactold.sa_handler != SIG_IGN)
		sigaction(SIGINT, &sigact, NULL);

	sigaction(SIGINT, NULL, &sigactold);
	if (sigactold.sa_handler != SIG_IGN)
		sigaction(SIGTERM, &sigact, NULL);
}

int main(int argc, char **argv)
{
	init_signal_handler();

	CLOG_INFO("VERSION %d", EWS_VERSION);
	return command_handler(argc, argv);
}
