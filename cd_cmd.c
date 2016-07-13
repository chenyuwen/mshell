#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "mshell.h"
#include "mstring.h"

int cd_command(struct mshell *mshell, struct cmd *cmd)
{
	unsigned char *path;
	int ret = 0;

	if(mshell->pipe >= 0) {
		return -1;
	}

	if(cmd->cmd[cmd->offset + 1] == NULL) {
		path = mshell->user->pw_dir;
	} else {
		path = cmd->cmd[cmd->offset + 1];
	}

	ret = chdir(path);
	if(ret < 0) {
		perror("cd");
	}
	getcwd(mshell->cur_dir, PATH_MAX);
	return ret;
}

