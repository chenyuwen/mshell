#ifndef __MSHELL_H__
#define __MSHELL_H__
#include <pwd.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

struct mshell {
	struct passwd *user;
	int main_loop;
	unsigned char *oneline;
	struct env {
		struct env *next;
		unsigned char *name;
		unsigned char *message;
	} *env;
};

struct cmd {
	unsigned char *cmd[256];
};

#endif /*__MSHELL_H__*/

