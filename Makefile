mshell: main.o mshell.o
	gcc main.o mshell.o -o mshell

main.o: main.c mshell.h
	gcc main.c -c -o main.o

mshell.o: mshell.c mshell.h
	gcc mshell.c -c -o mshell.o

.PHONY: clean
clean:
	rm -rf mshell *.o
