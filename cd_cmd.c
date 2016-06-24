#include <stdio.h>
#include <unistd.h>
#include "mshell.h"

int cd_command(struct mshell *mshell, struct cmd *cmd)
{
	unsigned char *path;
	int ret = 0;

	if(cmd->cmd[1] == NULL) {
		path = "~";
	} else {
		path = cmd->cmd[1];
	}

	/*TODO:把'~'替换为用户路径*/

	ret = chdir(path);
	if(ret < 0) {
		perror("mshell");
	}
	return ret;
}

