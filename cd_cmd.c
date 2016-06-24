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

	if(cmd->cmd[1] == NULL) {
		path = mshell->user->pw_dir;
	} else {
		path = cmd->cmd[1];
	}

	ret = chdir(path);
	if(ret < 0) {
		perror("mshell");
	}
	getcwd(mshell->cur_dir, PATH_MAX);
	return ret;
}

