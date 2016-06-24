mshell: main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o
	gcc main.o mshell.o internal_command.o cd_cmd.o exit_cmd.o -o mshell

main.o: main.c mshell.h
	gcc main.c -c -o main.o

mshell.o: mshell.c mshell.h
	gcc mshell.c -c -o mshell.o

internal_command.o: internal_command.c internal_command.h mshell.h
	gcc internal_command.c -c -o internal_command.o

cd_cmd.o: cd_cmd.c mshell.h
	gcc cd_cmd.c -c -o cd_cmd.o

exit_cmd.o: exit_cmd.c mshell.h
	gcc exit_cmd.c -c -o exit_cmd.o

.PHONY: clean
clean:
	rm -rf mshell *.o
