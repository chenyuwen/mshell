#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mshell.h"

static const struct cmd l_cmd = {
	"ls",
};

int l_command(struct mshell *mshell, struct cmd *cmd)
{
	mshell_handle_external_cmd(mshell, &l_cmd);
	return 0;
}

