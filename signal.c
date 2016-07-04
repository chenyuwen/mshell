#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <signal.h>

void signal_routine(int signal)
{
	switch(signal) {
		case SIGHUP:
			break;
		case SIGINT:
			break;
		case SIGQUIT:
			break;
	}
	printf("signal:%d\n", signal);
}

int signal_init(void)
{
	signal(SIGHUP, signal_routine);
	signal(SIGINT, signal_routine);
	signal(SIGQUIT, signal_routine);
	return 0;
}

