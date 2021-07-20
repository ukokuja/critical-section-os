//
// Created by lucas on 03/07/2021.
//

#pragma once
#include <stdio.h>

typedef struct MutexStruct {
    char state;
} Mutex;


void MutexInitPos(Mutex* m) {
    m->state=1;
};

/**
 * Initialize the mutex
 * @param m: Pointer to mutex instance
 */
void MutexInit(Mutex* m) {
   m->state=0;
};

/**
 * Request for getting mutex lock. Many can request it but one only can get it.
 * @param m: Pointer to mutex instance
 */
void MutexAcquire(Mutex* m) {
    while (!__sync_bool_compare_and_swap(&m->state, 0, 1))
        usleep(1);
};


/**
 * Releases mutex lock.
 * @param m: Pointer to mutex instance
 */
void MutexRelease(Mutex* m) {
    m->state = 0;
    __sync_synchronize();
}