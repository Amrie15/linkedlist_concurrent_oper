//
// Created by kiroshkumar on 01/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
//#define NUM_THREADS 4


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

double getExecutionTime(struct timeval time_begin, struct timeval time_end) {

    return (double) (time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double) (time_end.tv_sec - time_begin.tv_sec);
}


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

int main(int argc, char** argv) {
    int NUM_THREADS =atoi(argv[6]);
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    //TODO: get user input to get the percentage
    mMember = atof(argv[3]);
    mInsert = atof(argv[4]);
    mDelete = atof(argv[5]);

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
    printf("%.6f\n", getExecutionTime(time_begin, time_end));
//
//    printf("Execution time of mutex is : %.6f secs\n", getExecutionTime(time_begin, time_end));
//    printf("%d,%d,%d\n", countMemberOp, countInsertOp, countDeleteOp);
    return 0;
}


