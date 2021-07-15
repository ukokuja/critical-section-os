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
char reader_id = 'A';

void Reader(void) {
    int read_credits = 30;
    int my_reader_id = reader_id++;
    while(run && read_credits > 0) {
        SemDec(&r);
        __sync_synchronize();
        read_count++;
        __sync_synchronize();
        if (read_count == 1) SemDec(&rw);
        SemInc(&r);
        printf(">> Reading using reader %c - %d credits left >>\n", my_reader_id, read_credits);
        usleep(30000);
        printf("<< Reading using reader %c <<\n", my_reader_id);
        SemDec(&r);
        __sync_synchronize();
        read_count--;
        __sync_synchronize();
        if (read_count == 0) SemInc(&rw);
        SemInc(&r);
        read_credits--;
    }
    if (read_credits < 1) {
        printf("**** Reader %c have not credits ****\n", my_reader_id);
    }
}

void Writer(void) {
    while(run) {
        SemDec(&rw);
        __sync_synchronize();
        printf(">> Writing >>\n");
        x++;
        usleep(300000);
        printf("<< Writing <<\n");
        __sync_synchronize();
        SemInc(&rw);
    }
}

void* Worker(void *func) {
    long func_id = (long)func;
    sleep(1 * (int)func_id);
    switch (func_id) {
        case 0:
            printf("Starting writer\n");
            Writer();
            break;
        case 1:
        case 2:
        case 3:
            printf("Starting Reader\n");
            Reader();
            break;

    }
    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t t[argc];
    pthread_attr_t at;
    cpu_set_t cpuset;
    int threads = 4;
    int i;
    SemInit(&r, 1);
    SemInit(&rw, 1);
    for (i = 0;i < threads; i++){
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        pthread_attr_init(&at);
        pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
        if (pthread_create(&t[i], &at , Worker, (void *) (long)i)) {
            perror("pthread create 1 error\n");
        }
    }
    sleep(10);
    run = 0;
    void *val;
    for(i = 0; i < threads; i++)
        pthread_join(t[i], &val);
    printf("Final Output x = %d\n", x);
}