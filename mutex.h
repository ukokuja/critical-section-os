//
// Created by lucas on 03/07/2021.
//

#pragma once
#include <stdio.h>

typedef struct MutexStruct {
    char state;
} Mutex;


void MutexInit(Mutex* m) {
   m->state=0;
};

void MutexAcquire(Mutex* m) {
    while (!__sync_bool_compare_and_swap(&m->state, 0, 1));
    usleep(5);
};

void MutexRelease(Mutex* m) {
    m->state = 0;
}