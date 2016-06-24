#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

const struct passwd *get_user_passwd(void)
{
	struct passwd *passwd;

	passwd = getpwuid(getuid());
	if(NULL == passwd) {
		perror("mshell");
	}
	return passwd;
}

