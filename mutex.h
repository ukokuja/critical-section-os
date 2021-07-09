//
// Created by lucas on 09/07/2021.
//
#pragma once
#include <stdio.h>

//
// Created by lucas on 03/07/2021.
//
typedef struct MutexStruct {
    char state;
} Mutex;


void MutexInit(Mutex* m) {
   m->state=0;
};
void MutexInitPos(Mutex* m) {
   m->state=1;
};

void MutexAcquire(Mutex* m) {
    while (m->state <= 0 && !__sync_bool_compare_and_swap(&m->state, 0, 1));
    __sync_synchronize();
    usleep(5);
};

void MutexRelease(Mutex* m) {
    m->state = 0;
    __sync_synchronize();
}