all: make_test_mutex make_test_semaphore make_test_reader_writer

make_test_mutex: mutex/test_mutex.c ../mutex.h
	gcc -pthread mutex/test_mutex.c -o mutex/test_mutex

make_test_semaphore: semaphore/test_semaphore.c ../semaphore.h ../mutex.h
	gcc -pthread semaphore/test_semaphore.c -o semaphore/test_semaphore

make_test_reader_writer: reader_writer/test_reader_writer.c ../reader_writer.h ../semaphore.h ../mutex.h
	gcc -pthread reader_writer/test_reader_writer.c -o reader_writer/test_reader_writer

clean:
	rm semaphore/test_semaphore mutex/test_mutex reader_writer/test_reader_writer

test:
	cd mutex && ./test_mutex && cd ../semaphore &&./test_semaphore && cd ../reader_writer && ./test_reader_writer
