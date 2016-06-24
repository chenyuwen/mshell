#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int str_replace(unsigned char **out, unsigned char *src, 
		unsigned char *replace, unsigned char *dst) {
	unsigned char *buffer = NULL;
	int len = 0;
	unsigned char *pos = src, *last = src;

	len = strlen(src) + 1;
	while(pos = strstr(pos, replace)) {
		len += strlen(dst) - strlen(replace);
		if(buffer) {
			buffer = (void *)realloc(buffer, len);
		} else {
			buffer = (void *)malloc(len);
			buffer[0] = '\0';
		}

		strncat(buffer, last, pos - last);
		strcat(buffer, dst);
		pos += strlen(replace);
		last = pos;
	}

	if(buffer == NULL) {
		buffer = (void *)malloc(len);
		buffer[0] = '\0';
	}
	
	strcat(buffer, last);

	*out = buffer;
	return 0;
}

