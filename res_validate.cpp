//
// Created by test on 2023/2/23.
//
#include "ACC.h"

using namespace std;

int res_validate(int V_ID[], int V_Valid[]) {
    int count = 0;
    for (int i = 0; i < CORE_NUM; i++) {
        if (V_Valid[i]) {
            if (V_ID[i] % CORE_NUM != i) {
                exit(1);
            }
            count++;
        }
    }
    cout << count << endl;
    return count;
}