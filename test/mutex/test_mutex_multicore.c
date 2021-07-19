//
// Created by lucas on 13/07/2021.
//
#define _GNU_SOURCE
#include <sched.h>

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "../../mutex.h"
#include <string.h>

#define N 10  // number of threads
Mutex m;
int number = 0;
int numbers[N] = {0};


void *addTotal(void *x) {
    MutexAcquire(&m);
    numbers[number]++;
    usleep(10000);
    number++;
    MutexRelease(&m);
    return NULL;
}


void test_all_ones() {
    printf("test_all_ones\n");
    int i;
    for (i = 0; i < N; i++) {
        if (numbers[i] != 1) break;
    }
    assert(i == N);
    printf("✓ PASSED: test_all_ones\n");
}
void test_at_most_one() {
    printf("test_at_most_one\n");
    for (int i = 0; i < N; i++) {
        assert(numbers[i] < 2);
        assert(numbers[i] > 0);
    }
    printf("✓ PASSED: test_at_most_one\n");
}
void test_sum_equals_N() {
    printf("test_sum_equals_N\n");
    int total = 0;
    for (int i = 0; i < N; i++) {
        total += numbers[i];
    }
    assert(total == N);
    printf("✓ PASSED: test_sum_equals_N\n");
}
int main() {
    MutexInit(&m);
    pthread_t threads[N];
    pthread_attr_t at;
    cpu_set_t cpuset;
    for (int i = 0; i < N; i++) {
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        pthread_attr_init(&at);
        pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
        pthread_create(&threads[i], NULL, addTotal, (void*)&i);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sleep(1);
    printf("Starting mutex tests multi-core:\n");
    test_all_ones();
    test_at_most_one();
    test_sum_equals_N();
    printf("\n");
}