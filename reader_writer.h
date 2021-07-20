//
// Created by lucas on 19/07/2021.
//
#pragma once
#include "mutex.h"

typedef struct ReaderWriterStruct {
    Mutex mutex;
    Mutex rw_mutex;
    int read_count;
} ReaderWriter;

/**
 * Initialize Reader writer.
 * @param rw: Pointer to ReaderWriter instance
 */
void InitReaderWriter (ReaderWriter* rw) {
    MutexInit(&rw->rw_mutex);
    MutexInit(&rw->mutex);
    rw->read_count = 0;
}

/**
 * Acquires reader lock
 * @param rw: Pointer to ReaderWriter instance
 */
void ReadLock (ReaderWriter* rw) {
    MutexAcquire(&rw->mutex);
    __sync_synchronize();
    rw->read_count++;
    __sync_synchronize();
    if (rw->read_count == 1)
        MutexAcquire(&rw->rw_mutex);
    MutexRelease(&rw->mutex);
}

/**
 * Releases reader lock
 * @param rw: Pointer to ReaderWriter instance
 */
void ReadUnlock (ReaderWriter* rw) {
    MutexAcquire(&rw->mutex);
    __sync_synchronize();
    rw->read_count--;
    __sync_synchronize();
    if (rw->read_count == 0)
        MutexRelease(&rw->rw_mutex);
    MutexRelease(&rw->mutex);
}

/**
 * Acquires writer lock
 * @param rw: Pointer to ReaderWriter instance
 */
void WriteLock(ReaderWriter* rw){
    MutexAcquire(&rw->rw_mutex);
}

/**
 * Releases reader lock
 * @param rw: Pointer to ReaderWriter instance
 */
void WriteUnlock(ReaderWriter* rw){
    MutexRelease(&rw->rw_mutex);
}
