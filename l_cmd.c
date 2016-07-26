#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mshell.h"

static struct cmd l_cmd = {
	"ls", "--color=auto", NULL
};

int l_command(struct mshell *mshell, struct cmd *cmd)
{
	printf("herre\n");
	mshell_handle_external_cmd(mshell, &l_cmd);
	printf("out\n");
	return 0;
}

