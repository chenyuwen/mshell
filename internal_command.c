#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mshell.h"
#include "internal_command.h"

int cd_command(struct mshell *, struct cmd *);
int exit_command(struct mshell *, struct cmd *);
int ll_command(struct mshell *, struct cmd *);
int l_command(struct mshell *, struct cmd *);

static const struct internal_command internal_command[] = {
	[0] = {
		.name = "cd",
		.cmd = cd_command,
	},
	[1] = {
		.name = "exit",
		.cmd = exit_command,
	},
	[2] = {
		.name = "ll",
		.cmd = ll_command,
	},
	[3] = {
		.name = "l",
		.cmd = l_command,
	},
};


int mshell_is_internal_command(struct mshell *mshell, struct cmd *cmd)
{
	int i = 0;
	for(i = 0; i<ARRAY_SIZE(internal_command); i++) {
		if(!strcmp(internal_command[i].name, cmd->cmd[cmd->offset])) {
			return 1;
		}
	}
	return 0;
}

int mshell_handle_internal_cmd(struct mshell *mshell, struct cmd *cmd)
{
	int i = 0, offset = 0;
	int ret = -1;
	char *tmp = NULL;

	for(i=cmd->offset; cmd->cmd[i] != NULL; i++) {
		switch(cmd->cmd[i][0]) {
			case ';':
			case '#':
			case '$':
			case '|':
				goto out;
		}
	}

out:
	tmp = cmd->cmd[i];
	cmd->cmd[i] = NULL;
	offset = i;

	for(i=0; i<ARRAY_SIZE(internal_command); i++) {
		if(!strcmp(internal_command[i].name, cmd->cmd[cmd->offset])) {
			internal_command[i].cmd(mshell, cmd);
			ret = 0;
		}
	}

	if(tmp != NULL && tmp[0] == '#') {
		cmd->offset = cmd->max;
	} else {
		cmd->offset = offset + 1;
	}

	return ret;
}

