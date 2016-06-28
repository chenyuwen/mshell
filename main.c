#include <stdio.h>
#include <unistd.h>

#include "mshell.h"

int main(int argc, unsigned char **argv, unsigned char **envp)
{
	struct mshell mshell;
	struct cmd command;
	int i = 0;

	/*init mshell*/
	mshell_init(&mshell, argc, argv, envp);
	
//	for(i=0; i<argc; i++) {
//		printf("argv[%d] = %s\n", i, argv[i]);
//	}

//	while(envp[i] != NULL) {
//		printf("%s\n", envp[i++]);
//	}

	while(mshell.main_loop) {
		
		if(mshell.is_show_cmdline) {
			printf("%s:%s%s ", mshell.user->pw_name, mshell.cur_dir, 
				((mshell.user->pw_uid)?"$":"#"));
			fflush(stdout);
		}

		mshell_read_oneline(&mshell);

		mshell_parser_oneline(&mshell, &command);

		if(cmd_is_null(&mshell, &command)) {
			continue;
		}

		if(mshell_is_internal_command(&mshell, &command)) {
			mshell_handle_internal_cmd(&mshell, &command);
		} else {
			mshell_handle_external_cmd(&mshell, &command);
		}
		//mshell_free_command
	}
	
	return 0;
}


