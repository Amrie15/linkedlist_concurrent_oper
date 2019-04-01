//
// Created by kiroshkumar on 01/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

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

pthread_rwlock_t x;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct list_node_s {
    int data;
    struct list_node_s *next;
};

int member(int value, struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value) {
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

int insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL) {
            *head_pp = temp_p;
        } else {
            pred_p->next = temp_p;
        }
        return 1;

    } else {
        return 0;
    }
}

int delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data < value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;

    } else {
        return 0;
    }

}

void *thread_functions(void *arg) {
    while (Threshold < m) {
        int randomInt = rand() % max_range;

        if (Threshold < (mMember * m)) {

            pthread_rwlock_rdlock(&x);
                member(randomInt, head);
            pthread_rwlock_unlock(&x);
            //using a lock to ensure almost exact number of member operations are processed
            pthread_mutex_lock(&mutex);
            if (countMemberOp < (mMember * m)) {
              countMemberOp++;
              Threshold++;
            }
            pthread_mutex_unlock(&mutex);

            continue;
        } else if (Threshold < ((mMember * (m)) + (mInsert * (m)))) {
            pthread_rwlock_wrlock(&x);
            if (countInsertOp < (mInsert * m)) {
                insert(randomInt, &head);
                countInsertOp++;
                Threshold++;
            }

            pthread_rwlock_unlock(&x);

            continue;
        } else {
            pthread_rwlock_wrlock(&x);
            if (countDeleteOp < (mDelete * m)) {
                delete(randomInt, &head);
                countDeleteOp++;
                Threshold++;
            }
            pthread_rwlock_unlock(&x);

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

    srand(time(0));
    for (int i = 0; i < n; i++) {
        int temp = rand() % max_range;
        if (!insert(temp, &head)) {
            i--;
        }
    }


    int i;
    pthread_rwlock_init(&x, NULL);
    pthread_t tid[NUM_THREADS];
    clock_t begin = clock();
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, thread_functions, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    clock_t end = clock();
    double execution_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%d,%d,%d\n", countMemberOp, countInsertOp, countDeleteOp);
    printf("%lf",execution_time);



    return 0;
}


