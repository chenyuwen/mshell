#ifndef __MSHELL_H__
#define __MSHELL_H__
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

struct mshell {
	struct passwd *user;
	int main_loop;
	unsigned char *oneline;
	unsigned char cur_dir[PATH_MAX];
	int fd;
	int is_show_cmdline;
	struct env {
		struct env *next;
		unsigned char *name;
		unsigned char *message;
	} *env;
};

struct cmd {
	unsigned char *cmd[256];
	int offset;
};

#endif /*__MSHELL_H__*/

