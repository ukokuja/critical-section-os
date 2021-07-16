//
// Created by lucas on 09/07/2021.
//
#include "mutex.h"

typedef struct SemaphoreStruct{
    int val;
    Mutex mutex;
} Semaphore;

void SemInit(Semaphore* cs, int _val){
    cs->val = _val;
    MutexInit(&cs->mutex);
}

void SemDec(Semaphore* cs){
    MutexAcquire(&cs->mutex);
    __sync_synchronize();
    cs->val--;
    __sync_synchronize();
    while (cs->val < 0) {
        usleep(1000);
    }
    MutexRelease(&cs->mutex);
}

void SemInc(Semaphore* cs){
    cs->val++;
    __sync_synchronize();
}
