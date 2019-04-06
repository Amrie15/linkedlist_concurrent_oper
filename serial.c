//
// Created by kiroshkumar on 01/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

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

int main(int argc, char** argv) {
    struct list_node_s *head = NULL;
    struct timeval time_begin, time_end;

    int const n = atoi(argv[1]);
    int const m = atoi(argv[2]);




    float mMember = atof(argv[3]);
    float mInsert = atof(argv[4]);
    float mDelete = atof(argv[5]);


    int countMemberOp = 0;
    int countInsertOp = 0;
    int countDeleteOp = 0;

    //populate the linked- list
    int max_range = pow(2, 16);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        int temp = rand() % max_range;
        if (!insert(temp, &head)) {
            i--;
        };
    }

    gettimeofday(&time_begin, NULL);

    for (int j = 0; j < m; j++) {
        int randomInt = rand() % max_range;

        if (j < (mMember * m)) {
            member(randomInt, head);
            countMemberOp++;
            continue;
        } else if (j < ((mMember * m) + (mInsert * m))) {
            insert(randomInt, &head);
            countInsertOp++;
            continue;
        } else {
            delete(randomInt, &head);
            countDeleteOp++;
        }

    }
    gettimeofday(&time_end, NULL);
//    printf("Execution time of serial v : %.6f secs\n", getExecutionTime(time_begin, time_end));
    printf("%.6f\n", getExecutionTime(time_begin, time_end)*100);
//    printf("%d,%d,%d \n", countMemberOp, countInsertOp, countDeleteOp);


    return 0;
}