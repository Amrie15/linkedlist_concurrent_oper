//
// Created by kiroshkumar on 01/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "operations.h"

#define NUM_THREADS 4

int n;
int m;

struct list_node_s *head = NULL;

float mMember;
float mInsert;
float mDelete;

int countMemberOp = 0;
int countInsertOp = 0;
int countDeleteOp = 0;
int Threshold = 0;
int max_range;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_functions(void *arg) {
    while (Threshold < m) {
        int randomInt = rand() % max_range;

        if (Threshold < (mMember * m)) {
            pthread_mutex_lock(&mutex);
            if (Threshold < (mMember * m)) {
                member(randomInt, head);
                countMemberOp++;
                Threshold++;
            }

            pthread_mutex_unlock(&mutex);

            continue;
        } else if (Threshold < ((mMember * (m)) + (mInsert * (m)))) {
            pthread_mutex_lock(&mutex);
            if (countInsertOp < (mInsert * m)) {
                insert(randomInt, &head);
                countInsertOp++;
                Threshold++;
            }

            pthread_mutex_unlock(&mutex);

            continue;
        } else {
            pthread_mutex_lock(&mutex);
            if (Threshold < m) {
                delete(randomInt, &head);
                countDeleteOp++;
                Threshold++;
            }
            pthread_mutex_unlock(&mutex);

        }

    }
}

int main(void) {
    n = 1000;
    m = 10000;
    //TODO: get user input to get the percentage
    mMember = 0.99;
    mInsert = 0.005;
    mDelete = 0.005;

    max_range = pow(2, 16);
    struct timeval time_begin, time_end;

    srand(time(0));
    for (int i = 0; i < n; i++) {
        int temp = rand() % max_range;
        if (!insert(temp, &head)) {
            i--;
        }
    }

    int i;
    pthread_t tid[NUM_THREADS];
    gettimeofday(&time_begin, NULL);
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, thread_functions, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    gettimeofday(&time_end, NULL);
    printf("Execution time of mutex is : %.6f secs\n", CalcTime(time_begin, time_end));
    printf("%d,%d,%d\n", countMemberOp, countInsertOp, countDeleteOp);
    return 0;
}


