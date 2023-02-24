#include <iostream>
#include "ACC.h"

using namespace std;

int clk, rst_rd = 1;
int V_ID_IN[CORE_NUM], V_Valid_IN[CORE_NUM];
int V_ID_OUT[CORE_NUM], V_Valid_OUT[CORE_NUM], Network_Full[CORE_NUM];
int Next_Full[CORE_NUM];

int main() {
    int count = 0;
    for (clk = 0; clk < 10000; clk++) {
        if (clk == 10)rst_rd = 0;
        data_generate(Network_Full,
                      V_ID_IN, V_Valid_IN);

        Network(V_ID_IN, V_Valid_IN,
                Next_Full,

                Network_Full,
                V_ID_OUT, V_Valid_OUT);
        if (clk < 1000)
            res_validate(V_ID_OUT, V_Valid_OUT);
        else
            count += res_validate(V_ID_OUT, V_Valid_OUT);
    }
    cout << count << endl;
    return 0;
}
//Omega     512:0.936       256:0.937       128:0.941       64:0.942
//HyperX    512:0.537       256:0.55        128:0.564       64:0.595
//Torus     512:0.075       256:0.14        128:0.15        64:0.28