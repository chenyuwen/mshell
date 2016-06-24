mshell: main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o user.o
	gcc main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o user.o -o mshell

main.o: main.c mshell.h
	gcc main.c -c -o main.o

mshell.o: mshell.c mshell.h user.h
	gcc mshell.c -c -o mshell.o

internal_command.o: internal_command.c internal_command.h mshell.h
	gcc internal_command.c -c -o internal_command.o

cd_cmd.o: cd_cmd.c mshell.h
	gcc cd_cmd.c -c -o cd_cmd.o

exit_cmd.o: exit_cmd.c mshell.h
	gcc exit_cmd.c -c -o exit_cmd.o

user.o: user.c mshell.h
	gcc user.c -c -o user.o

.PHONY: clean
clean:
	rm -rf mshell *.o
