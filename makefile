all: make_test_mutex make_test_mutex_multicore make_test_semaphore make_test_semaphore_multicore make_test_reader_writer make_test_reader_writer_multicore

make_test_mutex: test/mutex/test_mutex.c mutex.h
	gcc -pthread test/mutex/test_mutex.c -o test/mutex/test_mutex

make_test_mutex_multicore: test/mutex/test_mutex_multicore.c mutex.h
	gcc -pthread test/mutex/test_mutex_multicore.c -o test/mutex/test_mutex_multicore

make_test_semaphore: test/semaphore/test_semaphore.c semaphore.h mutex.h
	gcc -pthread test/semaphore/test_semaphore.c -o test/semaphore/test_semaphore

make_test_semaphore_multicore: test/semaphore/test_semaphore_multicore.c semaphore.h mutex.h
	gcc -pthread test/semaphore/test_semaphore_multicore.c -o test/semaphore/test_semaphore_multicore

make_test_reader_writer: test/reader_writer/test_reader_writer.c reader_writer.h semaphore.h mutex.h
	gcc -pthread test/reader_writer/test_reader_writer.c -o test/reader_writer/test_reader_writer


make_test_reader_writer_multicore: test/reader_writer/test_reader_writer_multicore.c reader_writer.h semaphore.h mutex.h
	gcc -pthread test/reader_writer/test_reader_writer_multicore.c -o test/reader_writer/test_reader_writer_multicore

clean:
	rm test/semaphore/test_semaphore test/semaphore/test_semaphore_multicore test/mutex/test_mutex test/mutex/test_mutex_multicore test/reader_writer/test_reader_writer test/reader_writer/test_reader_writer_multicore

test_all:
	cd test/mutex && ./test_mutex && ./test_mutex_multicore && cd ../semaphore &&./test_semaphore &&./test_semaphore_multicore && cd ../reader_writer && ./test_reader_writer && ./test_reader_writer_multicore
