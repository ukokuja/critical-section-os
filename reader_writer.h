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

void InitReaderWriter (ReaderWriter* rw) {
    MutexInit(&rw->rw_mutex);
    MutexInit(&rw->mutex);
    rw->read_count = 0;
}

void ReadLock (ReaderWriter* rw) {
    MutexAcquire(&rw->mutex);
    __sync_synchronize();
    rw->read_count++;
    __sync_synchronize();
    if (rw->read_count == 1)
        MutexAcquire(&rw->rw_mutex);
    MutexRelease(&rw->mutex);
}

void ReadUnlock (ReaderWriter* rw) {
    MutexAcquire(&rw->mutex);
    __sync_synchronize();
    rw->read_count--;
    __sync_synchronize();
    if (rw->read_count == 0)
        MutexRelease(&rw->rw_mutex);
    MutexRelease(&rw->mutex);
}

void WriteLock(ReaderWriter* rw){
    MutexAcquire(&rw->rw_mutex);
}

void WriteUnlock(ReaderWriter* rw){
    MutexRelease(&rw->rw_mutex);
}
