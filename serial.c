//
// Created by kiroshkumar on 01/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "operations.h"


int main() {
    struct list_node_s *head = NULL;
    struct timeval time_begin, time_end;

    int const n = 1000;
    int const m = 10000;

    //TODO: get user input to get the percentage
    float mMember = 0.99;
    float mInsert = 0.005;
    float mDelete = 0.005;

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
    printf("Execution time of serial v : %.6f secs\n", CalcTime(time_begin, time_end));
    printf("%d,%d,%d \n", countMemberOp, countInsertOp, countDeleteOp);


    return 0;
}