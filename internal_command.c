#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mshell.h"
#include "internal_command.h"

int cd_command(struct mshell *, struct cmd *);
int exit_command(struct mshell *, struct cmd *);

static const struct internal_command internal_command[] = {
	[0] = {
		.name = "cd",
		.cmd = cd_command,
	},
	[1] = {
		.name = "exit",
		.cmd = exit_command,
	},
};


int mshell_is_internal_command(struct mshell *mshell, struct cmd *cmd)
{
	int i = 0;
	for(i = 0; i<ARRAY_SIZE(internal_command); i++) {
		if(!strcmp(internal_command[i].name, cmd->cmd[0])) {
			return 1;
		}
	}
	return 0;
}

int mshell_handle_internal_cmd(struct mshell *mshell, struct cmd *cmd)
{
	int i = 0;
	int ret = -1;
	for(i=0; i<ARRAY_SIZE(internal_command); i++) {
		if(!strcmp(internal_command[i].name, cmd->cmd[0])) {
			internal_command[i].cmd(mshell, cmd);
			ret = 0;
		}
	}
	return ret;
}

