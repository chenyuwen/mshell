#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mstring.h"
/*
int test_str_replace()
{
	unsigned char dst[] = "chenyuwen";
	unsigned char *out;

	str_replace(&out, dst, "chenyuwen", "EEE");
	str_replace(&out, dst, "chen", "EEE");

	printf("%s\n", out);
}
*/
#include<stdio.h>
#include<stdlib.h>
void moveyx(int y,int x)
{

	int i=0;
	char str[40]={
		0};
	sprintf(str,"\033[%d;%dH",y,x);
	fflush(stdout);
	printf("%sAAAAAAAA\n",str);
	fflush(stdout);
	printf("这里分割");
	fflush(stdout);
	printf("%sBBBBBB\n",str);
	fflush(stdout);
}

int main(int argc, unsigned char **argv)
{
/*	int fd = 0;
	close(1);
	fd = open("bin", O_CREAT | O_RDWR | 0666);
	
	printf("std.");
*/
	moveyx(2, 4);
	getchar();

//	test_str_replace();
	return 0;
}

