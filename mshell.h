#ifndef __MSHELL_H__
#define __MSHELL_H__

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))


struct mshell {
	const unsigned char *username;
	int is_root;
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

