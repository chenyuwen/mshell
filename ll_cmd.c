#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mshell.h"

static struct cmd ll_cmd = {
	"ls", "-l", "--color=auto", NULL,
};

int ll_command(struct mshell *mshell, struct cmd *cmd)
{
	mshell_handle_external_cmd(mshell, &ll_cmd);
	return 0;
}

