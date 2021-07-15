//
// Created by lucas on 09/07/2021.
//
#include "mutex.h"

typedef struct SemaphoreStruct{
    Mutex gate;
    Mutex mutex;
    int val;
} Semaphore;

void SemInit(Semaphore* cs, int _val){
    MutexInit(&cs->mutex);
    MutexInit(&cs->gate);
    cs->val = _val;
}

void SemDec(Semaphore* cs){
    MutexAcquire(&cs->gate);
    MutexAcquire(&cs->mutex);
    __sync_synchronize();
    cs->val--;
    __sync_synchronize();
    if (cs->val > 0){
        MutexRelease(&cs->gate);
    }
    MutexRelease(&cs->mutex);
}

void SemInc(Semaphore* cs){
    MutexAcquire(&cs->mutex);
    __sync_synchronize();
    cs->val++;
    __sync_synchronize();
    if (cs->val == 1){
        MutexRelease(&cs->gate);
    }
    MutexRelease(&cs->mutex);
}
