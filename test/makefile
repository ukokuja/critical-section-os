all: make_test_mutex make_test_semaphore

make_test_mutex: mutex/test_mutex.c ../mutex.h
	gcc -pthread mutex/test_mutex.c -o mutex/test_mutex

make_test_semaphore: semaphore/test_semaphore.c ../semaphore.h ../mutex.h
	gcc -pthread semaphore/test_semaphore.c -o semaphore/test_semaphore

clean:
	rm semaphore/test_semaphore mutex/test_mutex

test:
	cd mutex && ./test_mutex && cd ../semaphore &&./test_semaphore
