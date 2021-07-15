//
// Created by lucas on 13/07/2021.
//

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
    int i = 0;
    for (i = 0; i < N; i++) {
        if (numbers[i] != 1) break;
    }
    assert(i == N);
}
void test_at_most_one() {
    for (int i = 0; i < N; i++) {
        assert(numbers[i] < 2);
        assert(numbers[i] > 0);
    }
}
void test_sum_equals_N() {
    int total = 0;
    for (int i = 0; i < N; i++) {
        total += numbers[i];
    }
    assert(total == N);
}
int main() {
    MutexInit(&m);
    pthread_t threads[N];
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, addTotal, (void*)&i);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sleep(1);
    test_all_ones();
    test_at_most_one();
    test_sum_equals_N();
    printf("Test mutex passed :) \n");
}