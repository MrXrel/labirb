CFLAGS = -Wall -Wextra -pedantic -g -fdiagnostics-color=always
ARGS = 

all: final

valgrind: final
	valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all ./final $(ARGS)

final: functions.o main.o files.o string.o
	gcc $(CFLAGS) functions.o main.o files.o string.o -o final -lm

functions.o: functions.c
	gcc $(CFLAGS) -c functions.c;

string.o: string.c
	gcc $(CFLAGS) -c string.c;

main.o: main.c
	gcc $(CFLAGS) -c main.c;

clean:
	rm main.o functions.o string.o final