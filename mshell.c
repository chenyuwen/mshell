#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mshell.h"

int mshell_init(struct mshell *mshell)
{
	memset(mshell, 0, sizeof(struct mshell));
	strcpy(mshell->username, "Even");
	mshell->main_loop = 1;
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
		ret = read(0, &oneline[offset], 1);
		if(ret < 0) {
			perror("read");
			return -1;
		}

		if(oneline[offset] == '\n') {
			break;
		}

		offset++;
		if(offset == oneline_len) {
			oneline = (void *)realloc(oneline, oneline_len + 20);
			oneline_len += 20;
		}

		if(!oneline) {
			perror("realloc");
			return -1;
		}

	}

	mshell->oneline = oneline;	
	return 0;
}

int mshell_parser_oneline(const struct mshell *mshell, struct cmd *cmd)
{
	return 0;
}

int mshell_handle_cmd(struct mshell *mshell, const struct cmd *cmd)
{
	return 0;
}

