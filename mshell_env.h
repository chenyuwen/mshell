#ifndef __MSHELL_ENV_H__
#define __MSHELL_ENV_H__

#include "mshell.h"

struct mshell_env {
	char **env;
};

//int load_env(struct mshell *mshell, char **env);

//int cmd_prepare_env(struct mshell *mshell, struct cmd *cmd);

#endif /*__MSHELL_ENV_H__*/
