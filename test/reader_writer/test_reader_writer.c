#define _GNU_SOURCE 1

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "../../reader_writer.h"

int actions[512] = {-1};
int actions_index = 0;
int run = 1;
int x = 0;
ReaderWriter rw;
Mutex mutex_index;
char reader_id = 1;

void Reader(void) {
    int read_credits = 30;
    int my_reader_id = reader_id++;
    while (run && read_credits > 0) {
        ReadLock(&rw);

        MutexAcquire(&mutex_index);
        actions[actions_index] = my_reader_id;
        actions_index++;
        MutexRelease(&mutex_index);

        usleep(300000);

        MutexAcquire(&mutex_index);
        actions[actions_index] = my_reader_id + 10;
        actions_index++;
        MutexRelease(&mutex_index);

        ReadUnlock(&rw);
        read_credits--;
    }
}

void Writer(void) {
    int writer_id = 4;
    while (run) {
        usleep(1);
        WriteLock(&rw);

        MutexAcquire(&mutex_index);
        actions[actions_index] = writer_id;
        actions_index++;
        MutexRelease(&mutex_index);

        x++;
        usleep(300000);

        MutexAcquire(&mutex_index);
        actions[actions_index] = writer_id + 10;
        actions_index++;
        MutexRelease(&mutex_index);

        WriteUnlock(&rw);
    }
}

void *Worker(void *func) {
    long func_id = (long) func;
    sleep(1 * (int) func_id);
    switch (func_id) {
        case 0:
            Writer();
            break;
        case 1:
        case 2:
        case 3:
            Reader();
            break;

    }
    return NULL;
}

void test_multiple_readers_simultanous() {
    printf("test_multiple_readers_simultanous\n");
    int create_two_in_a_row = 0;
    for (int i = 0; i < actions_index - 1; i++) {
        if (((actions[i] == 1 || actions[i] == 2) && actions[i + 1] == 3) ||
            (actions[i] == 2 || actions[i] == 3) && actions[i + 1] == 1 ||
            (actions[i] == 1 || actions[i] == 3) && actions[i + 1] == 2) {
            create_two_in_a_row++;
        }
    }
    assert(create_two_in_a_row > 0);
    printf("✓ PASSED: test_multiple_readers_simultanous\n");
}

void test_no_readers_writer_simultanous() {
    printf("test_no_readers_writer_simultanous\n");
    for (int i = 0; i < actions_index - 1; i++) {
        if (actions[i] == 4)
            assert(actions[i + 1] == 14);
    }
    printf("✓ PASSED: test_no_readers_writer_simultanous\n");
}


int main(int argc, char *argv[]) {
    pthread_t t[argc];
    pthread_attr_t at;
    cpu_set_t cpuset;
    int threads = 4;
    int i;
    InitReaderWriter(&rw);
    MutexInit(&mutex_index);
    for (i = 0; i < threads; i++) {
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        pthread_attr_init(&at);
        pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
        if (pthread_create(&t[i], &at, Worker, (void *) (long) i)) {
            perror("pthread create 1 error\n");
        }
    }
    sleep(5);
    run = 0;
    void *val;
    for (i = 0; i < threads; i++)
        pthread_join(t[i], &val);

    printf("Starting reader_write tests:\n");
    test_multiple_readers_simultanous();
    test_no_readers_writer_simultanous();
    printf("\n");
}