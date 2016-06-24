#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

const char *get_cur_username(void)
{
	uid_t uid;
	struct passwd *passwd;

	uid = getuid();
	passwd = getpwuid(uid);
	if (NULL == passwd) {
		perror("mshell");
	}
	return passwd->pw_name;
}

