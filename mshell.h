#ifndef __MSHELL_H__
#define __MSHELL_H__
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

#include "mshell_env.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

struct mshell {
	struct passwd *user;
	int main_loop;
	unsigned char *oneline;
	unsigned char cur_dir[PATH_MAX];
	int fd;
	int pipe;
	int wait_for_childs;
	int is_show_cmdline;
	int (*read_oneline)(struct mshell *);

	struct mshell_env env;
};

struct cmd {
	unsigned char *cmd[256];
	int offset;
	int max;
};

#endif /*__MSHELL_H__*/

