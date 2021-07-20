//
// Created by lucas on 09/07/2021.
//
#include "mutex.h"

typedef struct SemaphoreStruct{
    int val;
    Mutex mutex;
} Semaphore;

/**
 * Initialize the Semaphore
 * @param cs: Pointer to semaphore instance
 * @param _val: Count limit
 */
void SemInit(Semaphore* cs, int _val){
    cs->val = _val;
    MutexInit(&cs->mutex);
}
/**
 * Decrements semaphore value. If the counter is lower than 0, it gets blocked
 * @param cs Pointer to semaphore instance
 */
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

/**
 * Increments the semaphore value. If before the increment the value was negative it
 * transfers a blocked process from the semaphore's waiting queue to the ready queue.
 * @param cs: Pointer to semaphore instance
 */
void SemInc(Semaphore* cs){
    cs->val++;
    __sync_synchronize();
}
