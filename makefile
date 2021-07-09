all: main

main: main.c semaphore.h mutex.h
	gcc -pthread main.c -o main

clean:
	rm main