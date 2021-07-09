#define _GNU_SOURCE 1
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "semaphore.h"

int run = 1;
int x = 0;
Semaphore r;
Semaphore rw;
int read_count = 0;
int amount_of_times_in_read_section = 0;
int amount_of_times_in_write_section = 0;

void Reader(void) {
    while(run) {
        SemDec(&r);
        read_count++;
        __sync_synchronize();
        if (read_count == 1) SemDec(&rw);
        SemInc(&r);
        amount_of_times_in_read_section++;
        SemDec(&r);
        read_count--;
        __sync_synchronize();
        if (read_count == 0) SemInc(&rw);
        SemInc(&r);
    }
}

void Writer(void) {
    while(run) {
        SemDec(&rw);
        x++;
        amount_of_times_in_write_section++;
        __sync_synchronize();
        SemInc(&rw);
    }
}

void* Worker(void *func) {
    SemInit(&r, 1);
    SemInit(&rw, 1);
    long func_id = (long)func & 0x1;
    printf("%s %d\n",__func__, (int)func_id);
    switch (func_id) {
        case 0:
            Reader();
            break;
        case 1:
            Writer();
            break;
    }
    return NULL;
}

#define MAX_PROCESSORS 4 // Minimal processors is 2.

int main(int argc, char *argv[]) {
    pthread_t t[argc];
    pthread_attr_t at;
    cpu_set_t cpuset;
    int threads;
    int i;
    threads = argc > 1 ? (( atoi(argv[1]) < 4) ? atoi(argv[1]):
                          MAX_PROCESSORS ) : 1;
    for (i = 0;i < threads; i++){
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        pthread_attr_init(&at);
        pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
        if (pthread_create(&t[i], &at , Worker, (void *) (long)i)) {
            perror("pthread create 1 error\n");
        }
    }
    do {
        sleep(1);
    } while(x < 0);
    run = 0;
    void *val;
    for(i = 0; i < threads; i++)
        pthread_join(t[i], &val);
    printf("Final Output x = %d\n", x);
    printf("Amount of times in read section = %d and amount of times in write section = %d\n",
           amount_of_times_in_read_section, amount_of_times_in_write_section);
}