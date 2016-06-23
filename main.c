#include <stdio.h>
#include <unistd.h>

#include "mshell.h"

int main(int argc, unsigned char **argv)
{
	struct mshell mshell;
	struct cmd command;

	/*init mshell*/
	mshell_init(&mshell);

	while(mshell.main_loop) {
		/*TODO: output shell tag.*/
		printf("%s%s:", mshell.username, (mshell.is_root?"#":"$"));
		fflush(stdout);

		mshell_read_oneline(&mshell);

		/*TODO: */
		mshell_parser_oneline(&mshell, &command);

		mshell_handle_cmd(&mshell, &command);
	}
	
	return 0;
}


