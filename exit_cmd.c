#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mshell.h"

int exit_command(struct mshell *mshell, struct cmd *cmd)
{
	exit(0);
	return 0;
}

