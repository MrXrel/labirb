CFLAGS = -Wall -Wextra -pedantic -g -fdiagnostics-color=always -O2

all: final

final: functions.o main.o
	gcc $(CFLAGS) functions.o main.o -o final -lm

functions.o: functions.c
	gcc $(CFLAGS) -c functions.c;

main.o: main.c
	gcc $(CFLAGS) -c main.c;

clean:
	rm main.o functions.o final