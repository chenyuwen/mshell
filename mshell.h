#ifndef __MSHELL_H__
#define __MSHELL_H__

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))


struct mshell {
	unsigned char username[20];
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
	unsigned char *cmd[64];
};

#endif /*__MSHELL_H__*/

