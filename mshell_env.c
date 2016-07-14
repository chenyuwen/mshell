#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mshell.h"
#include "mshell_env.h"
#include "mstring.h"

int load_env(struct mshell *mshell, char **env)
{
	mshell->env.env = env;
	return 0;
}

int string_prepare_env(struct mshell *mshell, char *env_name, int len, char **out)
{
	int i = 0, tag_len, ret = -1;
	char *tag_end;

	*out = NULL;
	for(i=0; mshell->env.env[i] != NULL; i++) {
		/*find tag*/
		tag_end = strchr(mshell->env.env[i], '=');
		if(tag_end == NULL) {
			continue;
		}

		tag_len = (unsigned long)tag_end - (unsigned long)mshell->env.env[i];
		if(tag_len <= 0) {
			continue;
		}

		if(tag_len != len) {
			continue;
		}

		/*compare*/
		if(!strncmp(env_name, mshell->env.env[i], len)) {
			*out = mshell->env.env[i] + tag_len + 1;
			ret = 0;
			break;
		}
	}

	return ret;
}

static int strlen_env(char *env)
{
	int i = 0;
	if(isdigit(env[0])) {
		while(env[i]!='\0'&&isdigit(env[++i]));
		return i;
	}

	for(i=0; env[i] != '\0'; i++) {
		if(!isdigit(env[i]) && !isalpha(env[i])) {
			break;
		}
	}
	return i;
}

int cmd_prepare_env(struct mshell *mshell, struct cmd *cmd)
{
	int i = 0, j = 0, ret = 0, len, offset;
	char *tmp = NULL, *out, *name;
	for(i=0; cmd->cmd[i] != NULL; i++) {

		tmp = cmd->cmd[i];

		while(tmp != NULL && tmp[0] != '\0') {
			tmp = strchr(tmp, '$');
			if(tmp == NULL) {
				break;
			}
			tmp += 1;
			if(tmp[0] == '$') {
				for(j=-1; tmp[j]!='\0'; j++) {
					tmp[j] = tmp[j+1];
					//tmp[j] = '\0';
				}
				tmp[j] = '\0';
				continue;
			}

			len = strlen_env(tmp);
			
			if(NULL != tmp && len > 0) {
				name = (void *) malloc(len + 2);
				strncpy(name, tmp - 1, len + 1);
				string_prepare_env(mshell, tmp, len, &out);

				if(out != NULL) {
					//mstring_replace();
					//printf("out:%s\n", out);
					offset = (unsigned long)tmp - (unsigned long)
						cmd->cmd[i];
					str_replace(&cmd->cmd[i], cmd->cmd[i],
                   				name, out);
					tmp = cmd->cmd[i] + offset;
				//	printf("name = %s, len %d\n", name, len);
				}
				free(name);
			}
		}

	}

	return 0;
}

