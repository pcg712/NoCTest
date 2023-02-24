//
// Created by test on 2023/2/23.
//

#ifndef NOCTEST_ACC_H
#define NOCTEST_ACC_H

#include <bits/stdc++.h>

const int CORE_NUM = 16;
const int FIFO_SIZE = 16;

const int OM_FIFO_SIZE = 16;
const int OMEGA_DEPTH = 8;
const int OMEGA_SWITCH_NUM = CORE_NUM / 2;

const int TO_FIFO_SIZE = 16;
const int Torus_X_NUM = 4;
const int Torus_Y_NUM = CORE_NUM / Torus_X_NUM;

const int HYPERX_DEPTH = 2;
const int VERTICAL_LEN = 8;
const int HORIZONTAL_LEN = 8;

void data_generate(int Next_Full[],
                   int *V_ID, int *V_DValid);

void Network(int Front_V_ID[], int Front_V_DValid[],
             int Source_Core_Full[],

             int *Stage_Full,
             int *V_ID, int *V_DValid);

int res_validate(int V_ID[], int V_Valid[]);

void Omega_Network(int Om1_Front_Update_V_ID[], int Om1_Front_Update_V_DValid[],
                   int Source_Core_Full[],

                   int *Stage_Full_Om1,
                   int *Om1_Update_V_ID, int *Om1_Update_V_DValid);

void Hyperx_Network(int Hy2_Front_Update_V_ID[], int Hy2_Front_Update_V_DValid[],
                    int Source_Core_Full[],

                    int *Stage_Full_Hy2,
                    int *Hy2_Update_V_ID, int *Hy2_Update_V_DValid);

void Torus_Network(int Om2_Front_Update_V_ID[], int Om2_Front_Update_V_DValid[],
                   int Source_Core_Full[],

                   int *Stage_Full_Om2,
                   int *Om2_Update_V_ID, int *Om2_Update_V_DValid);
#endif //NOCTEST_ACC_H