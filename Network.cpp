//
// Created by test on 2023/2/23.
//
#include "ACC.h"

void Network(int Front_V_ID[], int Front_V_DValid[],
             int Source_Core_Full[],

             int *Stage_Full,
             int *V_ID, int *V_DValid) {
//    Omega_Network(Front_V_ID, Front_V_DValid,
//                  Source_Core_Full,
//
//                  Stage_Full,
//                  V_ID, V_DValid);
//    Hyperx_Network(Front_V_ID, Front_V_DValid,
//                  Source_Core_Full,
//
//                  Stage_Full,
//                  V_ID, V_DValid);
    Torus_Network(Front_V_ID, Front_V_DValid,
                   Source_Core_Full,

                   Stage_Full,
                   V_ID, V_DValid);
}


