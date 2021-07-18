all: make_test_mutex make_test_semaphore make_test_reader_writer

make_test_mutex: test/mutex/test_mutex.c mutex.h
	gcc -pthread test/mutex/test_mutex.c -o test/mutex/test_mutex

make_test_semaphore: test/semaphore/test_semaphore.c semaphore.h mutex.h
	gcc -pthread test/semaphore/test_semaphore.c -o test/semaphore/test_semaphore

make_test_reader_writer: test/reader_writer/test_reader_writer.c reader_writer.h semaphore.h mutex.h
	gcc -pthread test/reader_writer/test_reader_writer.c -o test/reader_writer/test_reader_writer

clean:
	rm test/semaphore/test_semaphore test/mutex/test_mutex test/reader_writer/test_reader_writer

test_all:
	cd test/mutex && ./test_mutex && cd ../semaphore &&./test_semaphore && cd ../reader_writer && ./test_reader_writer
