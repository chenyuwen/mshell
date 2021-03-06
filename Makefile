mshell: main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o ll_cmd.o l_cmd.o user.o mstring.o signal.o mshell_env.o
	gcc main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o ll_cmd.o l_cmd.o user.o mstring.o signal.o mshell_env.o -o mshell

test: test.o mstring.o
	gcc test.o mstring.o -o test

main.o: main.c mshell.h
	gcc main.c -c -o main.o

mshell.o: mshell.c mshell.h user.h
	gcc mshell.c -c -o mshell.o

internal_command.o: internal_command.c internal_command.h mshell.h
	gcc internal_command.c -c -o internal_command.o

cd_cmd.o: cd_cmd.c mshell.h mstring.h
	gcc cd_cmd.c -c -o cd_cmd.o

exit_cmd.o: exit_cmd.c mshell.h
	gcc exit_cmd.c -c -o exit_cmd.o

ll_cmd.o: ll_cmd.c mshell.h
	gcc ll_cmd.c -c -o ll_cmd.o

l_cmd.o: l_cmd.c mshell.h
	gcc l_cmd.c -c -o l_cmd.o

user.o: user.c mshell.h
	gcc user.c -c -o user.o

mstring.o: mstring.c
	gcc mstring.c -c -o mstring.o

signal.o: signal.c
	gcc signal.c -c -o signal.o

mshell_env.o: mshell_env.c mshell_env.h mshell.h mstring.h
	gcc mshell_env.c -c -o mshell_env.o

.PHONY: clean
clean:
	rm -rf mshell *.o test

line:
	wc *.c *.h *.sh
