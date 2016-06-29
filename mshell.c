#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <fcntl.h>
#include "mshell.h"
#include "user.h"

int mshell_init(struct mshell *mshell, int argc, char **argv, char **env)
{
	memset(mshell, 0, sizeof(struct mshell));
	mshell->user = getpwuid(getuid());
	mshell->main_loop = 1;
	mshell->is_show_cmdline = 1;
	getcwd(mshell->cur_dir, PATH_MAX);
	mshell->fd = 0;

	/*需要执行脚本*/
	if(argc >= 2) {
		mshell->fd = open(argv[1], O_RDONLY);
		if(mshell->fd < 0) {
			mshell->main_loop = 0;
			perror("open");
			return 0;
		}
		mshell->is_show_cmdline = 0;
	}
	return 0;
}

int mshell_read_oneline(struct mshell *mshell)
{
	unsigned char *oneline;
	int offset = 0;
	int oneline_len = 20;
	int ret = 0;

	if(mshell->oneline) {
		free(mshell->oneline);
	}

	oneline = (void *)malloc(oneline_len);
	if(!oneline) {
		perror("malloc");
		return -1;
	}

	while(1) {
		ret = read(mshell->fd, &oneline[offset], 1);
		if(ret < 0) {
			perror("read");
			return -1;
		} else if(!ret) {
			mshell->main_loop = 0;
			return 0;
		}

		if(oneline[offset] == '\n') {
			oneline[offset] = '\0';
			break;
		}

		if(oneline[offset] == '\t') {
			printf("/b");
		}

		offset++;
		if(offset >= (oneline_len - 1)) {
			oneline = (void *)realloc(oneline, oneline_len + 20);
			if(oneline == NULL) {
				perror("realloc");
				return -1;
			}
			oneline_len += 20;
		}
	}

	mshell->oneline = oneline;
	return 0;
}

int parser_one_block(unsigned char *oneline, int *oneline_offset, unsigned char **out)
{
	unsigned char *block = NULL;
	int has_quotation_mark = 0;
	int last_is_escape = 0;
	int offset = *oneline_offset;
	int skip_offset = 0;
	int out_offset = 0, out_len = 0;

	/*filter space*/
	while(oneline[offset] == ' ' || oneline[offset] == '\n' || oneline[offset] == ';')
		offset++;

	skip_offset = offset;
	if(oneline[offset] == '\"' || oneline[offset] == '\'') {
		has_quotation_mark = 1;
		offset++;
	}

	while(oneline[offset] != '\0') {
		if(out_offset >= (out_len - 1)) {
			out_len += 20;
			if(block == NULL) {
				block = (void *)malloc(out_len);
			} else {
				block = (void *)realloc((void *)block, out_len);
			}
		}

		if(last_is_escape) {
			/*TODO: save char*/
			block[out_offset++] = oneline[offset++];
			last_is_escape = 0;
			continue;
		}

		switch(oneline[offset]) {
			case '\\':
				last_is_escape = 1;
				offset++;
				continue;
			case '\"':
			case '\'':
				if(has_quotation_mark) {
					has_quotation_mark = 0;
				} else {
					has_quotation_mark = 1;
				}
				offset++;
				continue;
			case ' ':
			case '\n':
			case ';':
				if(!has_quotation_mark) {
					goto out;
				}
				break;
		}

		block[out_offset++] = oneline[offset++];
	}

out:
	if(has_quotation_mark || last_is_escape) {
		free(block);
		*out = NULL;
		return -1;
	}

	if(skip_offset == offset) {
		free(block);
		block = NULL;
	} else {
		block[out_offset] = '\0';
	}

	*oneline_offset = offset;
	*out = block;
	return 0;
}

int mshell_parser_oneline(const struct mshell *mshell, struct cmd *cmd)
{
	int offset = 0;
	unsigned char *out, *tmp;
	int i = 0;

	for(i=0; i<ARRAY_SIZE(cmd->cmd); i++) {
		if(cmd->cmd[i] != NULL) {
			free(cmd->cmd[i]);
			cmd->cmd[i] = NULL;
		} else {
			break;
		}
	}

	/*parser command*/
	do {
		parser_one_block(mshell->oneline, &offset, &out);
		//printf("out: offset = %d, %s\n", offset, out);
		//fflush(stdout);
		
		//if(out != NULL) {
		//	free(out);
		//}
		if(out != NULL) {
			tmp = out;
			str_replace(&out, tmp, "~", mshell->user->pw_dir);
			free(tmp);
		} 
		cmd->cmd[i++] = out;
	} while(out != NULL);

	/*TODO: parser arg*/
	return 0;
}

int cmd_is_null(struct mshell *mshell, const struct cmd *cmd)
{
	if(cmd->cmd[0] == NULL) {
		return 1;
	}
	return 0;
}

int mshell_handle_external_cmd(struct mshell *mshell, const struct cmd *cmd)
{
	int ret = 0;
	int pid = 0;
	if(cmd->cmd[0] != NULL) {

		pid = fork();

		if(pid == 0) {
			ret = execvp(cmd->cmd[0], (void *)cmd->cmd);
			if(ret < 0) {
				perror(cmd->cmd[0]);
			}

			exit(ret);
		} else if(pid > 0) {
			wait(&ret);
		} else {
			perror("fork");
		}
	}

exit:
	return ret;
}

