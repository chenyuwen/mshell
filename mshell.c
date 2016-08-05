#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <termios.h>
#include <fcntl.h>
#include "mshell.h"
#include "user.h"

#define MOVELEFT() do{printf("\033[D");}while(0)

int mshell_read_oneline(struct mshell *mshell);
int mshell_read_oneline_from_file(struct mshell *mshell);

int mshell_init(struct mshell *mshell, int argc, char **argv, char **env)
{
	memset(mshell, 0, sizeof(struct mshell));
	mshell->user = getpwuid(getuid());
	mshell->main_loop = 1;
	mshell->is_show_cmdline = 1;
	getcwd(mshell->cur_dir, PATH_MAX);
	mshell->fd = 0;
	mshell->pipe = -1;
	mshell->wait_for_childs = 0;

	mshell->read_oneline = mshell_read_oneline;
	/*需要执行脚本*/
	if(argc >= 2) {
		mshell->fd = open(argv[1], O_RDONLY);
		if(mshell->fd < 0) {
			mshell->main_loop = 0;
			perror("open");
			return 0;
		}
		mshell->is_show_cmdline = 0;
		mshell->read_oneline = mshell_read_oneline_from_file;
	}

	signal_init();
	load_env(mshell, env);
	return 0;
}

int escape_up_down(int offset, int *cur_offset)
{
	char keycode[4];

	keycode[0] = '\033';
	keycode[1] = getchar();
	keycode[2] = getchar();
	keycode[3] ='\0';

	if(!strncmp(keycode, "\033[D", 3)) {
		if(*cur_offset > 0) {
			printf("%s", keycode);
			//printf("%d", *cur_offset);
			(*cur_offset)--;
			//printf("LEFT %d %d", *cur_offset, offset);
		}
	} else if(!strncmp(keycode, "\033[C", 3)) {
		if(*cur_offset < offset) {
			printf("%s", keycode);
			(*cur_offset)++;
			//printf("RIGHT %d %d", *cur_offset, offset);
		}
	}
	return 0;
}

int mshell_read_oneline(struct mshell *mshell)
{
	unsigned char *oneline;
	char chr;
	int offset = 0;
	int oneline_len = 20;
	int ret = 0;
	int cur_offset = 0;
	int i = 0;
	struct termios old, new;

	if(mshell->oneline) {
		free(mshell->oneline);
	}

	oneline = (void *)malloc(oneline_len);
	if(!oneline) {
		perror("malloc");
		return -1;
	}

	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new);

	while(1) {
		chr = getchar();

		switch(chr) {
			case '\n':
			case '\0':
				putchar('\n');
				goto exit;
				break;
			case '\t':
				continue;
			case 127: /*DEL*/
				if(cur_offset > 0) {
					MOVELEFT();
					oneline[offset] = '\0';
					printf("%s ", &oneline[cur_offset]);
					for(i=cur_offset; i<offset; i++) {
						MOVELEFT();
						oneline[i - 1] = oneline[i];
					}
					MOVELEFT();
					oneline[offset - 1] = '\0';
					cur_offset--;
					offset--;
				}
				continue;
			case '\033':
				//printf("cur %d", cur_offset);
				escape_up_down(offset, &cur_offset);
				continue;
			default:
				//printf("%d\n", oneline[offset]);
				oneline[offset + 1] = '\0';
				for(i=cur_offset; i<offset; i++) oneline[i + 1] = oneline[i];
				oneline[cur_offset] = chr;
				oneline[offset + 2] = '\0';
				printf("%s", &oneline[cur_offset]);
				for(i=cur_offset; i<offset; i++) MOVELEFT();
				//oneline[offset] = chr;
				cur_offset++;
				break;

		}

		offset++;
		if(offset >= (oneline_len - 1)) {
			oneline = (void *)realloc(oneline, oneline_len + 20);
			if(oneline == NULL) {
				perror("realloc");
				tcsetattr(STDIN_FILENO, TCSANOW, &old);
				return -1;
			}
			oneline_len += 20;
		}
	}

exit:
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	mshell->oneline = oneline;
	return 0;
}

int mshell_read_oneline_from_file(struct mshell *mshell)
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
			oneline[offset] = '\0';
			goto exit;
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

exit:
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
	while(oneline[offset] == ' ' || oneline[offset] == '\n')
		offset++;

	skip_offset = offset;
	if(oneline[offset] == '\"' || oneline[offset] == '\'') {
		has_quotation_mark = 1;
		offset++;
	}

	if(oneline[offset] == ';' || oneline[offset] == '|' ||
			oneline[offset] == '#' || oneline[offset] == '&') {
		block = (void *)malloc(2);
		block[0] = oneline[offset++];
		block[1] = '\0';
		out_offset++;
		goto out;
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
			case '|':
			case '&':
			case '#':
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
			i = 0;
			break;
		}
	}

	cmd->offset = 0; /*init offset*/

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
			//printf("%s\n", out);
		}
		cmd->cmd[i++] = out;
		cmd->max = i;
	} while(out != NULL);

	/*TODO: parser arg*/
	return 0;
}

int cmd_is_null(struct mshell *mshell, const struct cmd *cmd)
{
	if(cmd->cmd[cmd->offset] == NULL) {
		return 1;
	}
	return 0;
}

int mshell_handle_external_cmd(struct mshell *mshell, struct cmd *cmd)
{
	int ret = 0;
	int pid = 0;
	int offset = cmd->offset, i;
	unsigned char *tmp;
	int pipefd[2] = {-1, -1};

	if(cmd->cmd[offset] != NULL) {
		/*TODO:准备参数, offset指向下一个*/
		for(i=offset; cmd->cmd[i] != NULL; i++) {
			switch(cmd->cmd[i][0]) {
				case ';':
				case '#':
				case '$':
					goto out;
				case '|':
					ret = pipe(pipefd);
					if(ret < 0) {
						perror("pipe");
						return -1;
					}
					goto out;
			}
		}

out:
		tmp = cmd->cmd[i];
		cmd->cmd[i] = NULL;
		pid = fork();

		if(pid == 0) {
			if(mshell->pipe != -1) {
				close(0); //close input
				dup2(mshell->pipe, 0);
			}

			if(pipefd[1] >= 0) {
				close(pipefd[0]);
				close(1); // close output
				dup2(pipefd[1], 1); // ouput > pipe out
			}

			ret = execvp(cmd->cmd[offset], (void *)&cmd->cmd[cmd->offset]);
			if(ret < 0) {
				perror(cmd->cmd[offset]);
			}

			exit(ret);
		} else if(pid > 0) {
			if(tmp == NULL || (tmp != NULL && tmp[0] != '|')) {
				mshell->wait_for_childs++;
				while(mshell->wait_for_childs) {
					wait(&ret);
					mshell->wait_for_childs--;
				}
			} else {
				mshell->wait_for_childs++;
			}
			//wait(&ret);
			cmd->cmd[i] = tmp;
			mshell->pipe = pipefd[0];
			if(pipefd[1] >= 0) {
				close(pipefd[1]);
			}

			if(tmp != NULL && tmp[0] == '#') {
				cmd->offset = cmd->max;
			} else {
				cmd->offset = i + 1;
			}
		} else {
			perror("fork");
		}
	}

exit:
	return ret;
}

