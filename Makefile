CFLAG += -g

mshell: main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o ll_cmd.o l_cmd.o user.o mstring.o signal.o mshell_env.o
	gcc $(CFLAG)  main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o ll_cmd.o l_cmd.o user.o mstring.o signal.o mshell_env.o -o mshell

test: test.o mstring.o
	gcc $(CFLAG)  test.o mstring.o -o test

main.o: main.c mshell.h
	gcc $(CFLAG) main.c -c -o main.o

mshell.o: mshell.c mshell.h user.h
	gcc $(CFLAG) mshell.c -c -o mshell.o

internal_command.o: internal_command.c internal_command.h mshell.h
	gcc $(CFLAG) internal_command.c -c -o internal_command.o

cd_cmd.o: cd_cmd.c mshell.h mstring.h
	gcc $(CFLAG) cd_cmd.c -c -o cd_cmd.o

exit_cmd.o: exit_cmd.c mshell.h
	gcc $(CFLAG) exit_cmd.c -c -o exit_cmd.o

ll_cmd.o: ll_cmd.c mshell.h
	gcc $(CFLAG) ll_cmd.c -c -o ll_cmd.o

l_cmd.o: l_cmd.c mshell.h
	gcc $(CFLAG) l_cmd.c -c -o l_cmd.o

user.o: user.c mshell.h
	gcc $(CFLAG) user.c -c -o user.o

mstring.o: mstring.c
	gcc $(CFLAG) mstring.c -c -o mstring.o

signal.o: signal.c
	gcc $(CFLAG) signal.c -c -o signal.o

mshell_env.o: mshell_env.c mshell_env.h mshell.h mstring.h
	gcc $(CFLAG) mshell_env.c -c -o mshell_env.o

.PHONY: clean
clean:
	rm -rf mshell *.o test

line:
	wc *.c *.h *.sh
