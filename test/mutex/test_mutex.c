//
// Created by lucas on 12/07/2021.
//
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include "../../mutex.h"
#include <string.h>

Mutex m;
int lineNumber = 0;

void *copyFile(void *x) {
    MutexAcquire(&m);
    char line[256];
    FILE *from = fopen("from.txt","r");
    FILE *to = fopen("to.txt","a");
    if (from == NULL || to == NULL)
        exit(1);

    fseek( from, (long)lineNumber, SEEK_SET );
    fgets(line, sizeof(line), from);
    fprintf(to,"%s\n",line);
    fclose(from);
    fclose(to);
    lineNumber++;
    MutexRelease(&m);
    return NULL;
}

void test_expected () {
    FILE *to = fopen("to.txt", "r");
    FILE *expected = fopen("expected.txt", "r");
    char *line_to = NULL;
    char *line_expected = NULL;
    size_t len_to = 0;
    size_t len_expected = 0;
    ssize_t read_to, read_expected;
    if (to == NULL) {
        printf("ERROR\n");
        exit(1);
    }

    while ((read_to = getline(&line_to, &len_to, to)) != -1 && (read_expected = getline(&line_expected, &len_expected, expected)) != -1) {
        assert(strcmp(line_to, line_expected) == 0);
    }
    printf("Test mutex passed :)\n");
    fclose(to);
    fclose(fopen("to.txt", "w"));
    fclose(expected);
}

int main() {
    const int N = 10;  // number of threads
    pthread_t threads[N];
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, copyFile, (void *)&i);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sleep(1);
    test_expected();
}