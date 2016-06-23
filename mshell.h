#ifndef __MSHELL_H__
#define __MSHELL_H__

struct mshell {
	unsigned char username[20];
	int is_root;
	unsigned char **cmdline;
	int main_loop;
	unsigned char *oneline;
};

struct cmd {
};

#endif /*__MSHELL_H__*/

