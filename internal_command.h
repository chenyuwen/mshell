#ifndef __INTERNAL_COMMAND_H__
#define __INTERNAL_COMMAND_H__

struct internal_command {
	unsigned char *name;
	int (*cmd)(struct mshell *, struct cmd *);
};

#endif /*__INTERNAL_COMMAND_H__*/

