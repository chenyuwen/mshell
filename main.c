#include <stdio.h>
#include <unistd.h>

#include "mshell.h"

int main(int argc, unsigned char **argv, unsigned char **envp)
{
	struct mshell mshell;
	struct cmd command;
	int i = 0;

	/*init mshell*/
	mshell_init(&mshell);

	while(envp[i] != NULL) {
		printf("%s\n", envp[i++]);
	}

	while(mshell.main_loop) {
		/*TODO: output shell tag.*/
		printf("%s%s:", mshell.username, (mshell.is_root?"#":"$"));
		fflush(stdout);

		mshell_read_oneline(&mshell);

		/*TODO: */
		mshell_parser_oneline(&mshell, &command);

		if(mshell_is_internal_command(&mshell, &command)) {
			mshell_handle_internal_cmd(&mshell, &command);
		} else {
			mshell_handle_external_cmd(&mshell, &command);
		}
	}
	
	return 0;
}


