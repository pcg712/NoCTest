//
// Created by test on 2023/2/23.
//
#include "ACC.h"

extern int rst_rd;

void data_generate(int Next_Full[],
                   int *V_ID, int *V_DValid) {
    //srand((unsigned int) (time(NULL)));
    for (int i = 0; i < CORE_NUM; i++) {
        if (Next_Full[i] || rst_rd) {
            V_ID[i] = 0;
            V_DValid[i] = 0;
        } else {
            V_ID[i] = rand();
            V_DValid[i] = 1;
        }
    }
}