#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mstring.h"

int test_str_replace()
{
	unsigned char dst[] = "chenyuwen";
	unsigned char *out;

	str_replace(&out, dst, "chenyuwen", "EEE");
	str_replace(&out, dst, "chen", "EEE");

	printf("%s\n", out);
}

int main(int argc, unsigned char **argv)
{
	int fd = 0;
	close(1);
	fd = open("bin", O_CREAT | O_RDWR | 0666);
	
	printf("std.");

	test_str_replace();
	return 0;
}

