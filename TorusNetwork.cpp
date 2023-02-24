//
// Created by test on 2023/2/23.
//
//
// Created by test on 2023/2/20.
//
#include "ACC.h"

extern int clk;
extern int rst_rd;

using namespace std;
void Torus_Network_2_Single(int Torus_Node_ID, int Torus_Node_ID_X, int Torus_Node_ID_Y,
                            int V_ID_In, int V_Valid_In,
                            int V_ID_Left, int V_Valid_Left,
                            int V_ID_Up, int V_Valid_Up,
                            int Next_Stage_Full1, int Next_Stage_Full2, int Next_Stage_Full3,

                            int *Stage_Full1, int *Stage_Full2, int *Stage_Full3,
                            int *To2_V_ID_Out, int *To2_V_Valid_Out,
                            int *To2_V_ID_Right, int *To2_V_Valid_Right,
                            int *To2_V_ID_Down, int *To2_V_Valid_Down);

void Torus_Network(int Om2_Front_Update_V_ID[], int Om2_Front_Update_V_DValid[],
                   int Source_Core_Full[],

                   int *Stage_Full_Om2,
                   int *Om2_Update_V_ID, int *Om2_Update_V_DValid) {

    static int tmp_to1_push_flag_in[CORE_NUM], tmp_to1_v_id_in[CORE_NUM], tmp_to1_v_value_in[CORE_NUM], tmp_to1_pull_first_flag_in[CORE_NUM], tmp_to1_v_valid_in[CORE_NUM],
            tmp_to1_iteration_end_in[CORE_NUM], tmp_to1_iteration_end_dvalid_in[CORE_NUM];
    static int tmp_to1_push_flag_left[CORE_NUM], tmp_to1_v_id_left[CORE_NUM], tmp_to1_v_value_left[CORE_NUM], tmp_to1_pull_first_flag_left[CORE_NUM], tmp_to1_v_valid_left[CORE_NUM],
            tmp_to1_iteration_end_left[CORE_NUM], tmp_to1_iteration_end_dvalid_left[CORE_NUM];
    static int tmp_to1_push_flag_up[CORE_NUM], tmp_to1_v_id_up[CORE_NUM], tmp_to1_v_value_up[CORE_NUM], tmp_to1_pull_first_flag_up[CORE_NUM], tmp_to1_v_valid_up[CORE_NUM],
            tmp_to1_iteration_end_up[CORE_NUM], tmp_to1_iteration_end_dvalid_up[CORE_NUM];
    static int tmp_to2_push_flag_in[CORE_NUM], tmp_to2_v_id_in[CORE_NUM], tmp_to2_v_value_in[CORE_NUM], tmp_to2_pull_first_flag_in[CORE_NUM], tmp_to2_v_valid_in[CORE_NUM],
            tmp_to2_iteration_end_in[CORE_NUM], tmp_to2_iteration_end_dvalid_in[CORE_NUM];
    static int tmp_to2_push_flag_left[CORE_NUM], tmp_to2_v_id_left[CORE_NUM], tmp_to2_v_value_left[CORE_NUM], tmp_to2_pull_first_flag_left[CORE_NUM], tmp_to2_v_valid_left[CORE_NUM],
            tmp_to2_iteration_end_left[CORE_NUM], tmp_to2_iteration_end_dvalid_left[CORE_NUM];
    static int tmp_to2_push_flag_up[CORE_NUM], tmp_to2_v_id_up[CORE_NUM], tmp_to2_v_value_up[CORE_NUM], tmp_to2_pull_first_flag_up[CORE_NUM], tmp_to2_v_valid_up[CORE_NUM],
            tmp_to2_iteration_end_up[CORE_NUM], tmp_to2_iteration_end_dvalid_up[CORE_NUM];

    static int tmp_to1_stage_full1[CORE_NUM], tmp_to1_stage_full2[CORE_NUM], tmp_to1_stage_full3[CORE_NUM];
    static int tmp_to2_stage_full1[CORE_NUM], tmp_to2_stage_full2[CORE_NUM], tmp_to2_stage_full3[CORE_NUM];

    for (int i = 0; i < CORE_NUM; ++ i) {
        // check power-law vertex

        tmp_to2_v_id_in[i] = Om2_Front_Update_V_ID[i];
        tmp_to2_v_valid_in[i] = Om2_Front_Update_V_DValid[i];
        tmp_to2_stage_full1[i] = Source_Core_Full[i]; //destination core never full
    }

    for(int i = 0; i < CORE_NUM; i++){
        int x_id = i % Torus_X_NUM;
        int y_id = i / Torus_X_NUM;
        int left_x_id = x_id == 0 ? Torus_X_NUM - 1 : x_id - 1;
        int up_y_id = y_id == 0 ? Torus_Y_NUM - 1 : y_id - 1;
        int left_id = y_id * Torus_X_NUM + left_x_id;
        int up_id = up_y_id * Torus_X_NUM + x_id;
        Torus_Network_2_Single(i, x_id, y_id,
                               tmp_to2_v_id_in[i], tmp_to2_v_valid_in[i],
                               tmp_to2_v_id_left[left_id], tmp_to2_v_valid_left[left_id],
                               tmp_to2_v_id_up[up_id], tmp_to2_v_valid_up[up_id],
                               tmp_to2_stage_full1[i], tmp_to2_stage_full2[i], tmp_to2_stage_full3[i],

                               &tmp_to2_stage_full1[i], &tmp_to2_stage_full2[left_id], &tmp_to2_stage_full3[up_id],
                               &Om2_Update_V_ID[i], &Om2_Update_V_DValid[i],
                               &tmp_to2_v_id_left[i], &tmp_to2_v_valid_left[i],
                               &tmp_to2_v_id_up[i], &tmp_to2_v_valid_up[i]);
    }

    for (int i = 0; i < CORE_NUM; ++ i) {
        Stage_Full_Om2[i] = tmp_to2_stage_full1[i];
    }
}

void Torus_Network_2_Single(int Torus_Node_ID, int Torus_Node_ID_X, int Torus_Node_ID_Y,
                            int V_ID_In, int V_Valid_In,
                            int V_ID_Left, int V_Valid_Left,
                            int V_ID_Up, int V_Valid_Up,
                            int Next_Stage_Full1, int Next_Stage_Full2, int Next_Stage_Full3,

                            int *Stage_Full1, int *Stage_Full2, int *Stage_Full3,
                            int *To2_V_ID_Out, int *To2_V_Valid_Out,
                            int *To2_V_ID_Right, int *To2_V_Valid_Right,
                            int *To2_V_ID_Down, int *To2_V_Valid_Down) {
    static queue<int> push_flag_buffer_in_out[CORE_NUM], id_buffer_in_out[CORE_NUM], value_buffer_in_out[CORE_NUM], pull_first_flag_buffer_in_out[CORE_NUM];
    static queue<int> push_flag_buffer_in_right[CORE_NUM], id_buffer_in_right[CORE_NUM], value_buffer_in_right[CORE_NUM], pull_first_flag_buffer_in_right[CORE_NUM];
    static queue<int> push_flag_buffer_in_down[CORE_NUM], id_buffer_in_down[CORE_NUM], value_buffer_in_down[CORE_NUM], pull_first_flag_buffer_in_down[CORE_NUM];
    static queue<int> push_flag_buffer_left_out[CORE_NUM], id_buffer_left_out[CORE_NUM], value_buffer_left_out[CORE_NUM], pull_first_flag_buffer_left_out[CORE_NUM];
    static queue<int> push_flag_buffer_left_right[CORE_NUM], id_buffer_left_right[CORE_NUM], value_buffer_left_right[CORE_NUM], pull_first_flag_buffer_left_right[CORE_NUM];
    static queue<int> push_flag_buffer_left_down[CORE_NUM], id_buffer_left_down[CORE_NUM], value_buffer_left_down[CORE_NUM], pull_first_flag_buffer_left_down[CORE_NUM];
    static queue<int> push_flag_buffer_up_out[CORE_NUM], id_buffer_up_out[CORE_NUM], value_buffer_up_out[CORE_NUM], pull_first_flag_buffer_up_out[CORE_NUM];
    static queue<int> push_flag_buffer_up_right[CORE_NUM], id_buffer_up_right[CORE_NUM], value_buffer_up_right[CORE_NUM], pull_first_flag_buffer_up_right[CORE_NUM];
    static queue<int> push_flag_buffer_up_down[CORE_NUM], id_buffer_up_down[CORE_NUM], value_buffer_up_down[CORE_NUM], pull_first_flag_buffer_up_down[CORE_NUM];

    static int buffer_empty_in_out[CORE_NUM], buffer_empty_in_right[CORE_NUM], buffer_empty_in_down[CORE_NUM];
    static int buffer_empty_left_out[CORE_NUM], buffer_empty_left_right[CORE_NUM], buffer_empty_left_down[CORE_NUM];
    static int buffer_empty_up_out[CORE_NUM], buffer_empty_up_right[CORE_NUM], buffer_empty_up_down[CORE_NUM];
    static int buffer_full_in_out[CORE_NUM], buffer_full_in_right[CORE_NUM], buffer_full_in_down[CORE_NUM];
    static int buffer_full_left_out[CORE_NUM], buffer_full_left_right[CORE_NUM], buffer_full_left_down[CORE_NUM];
    static int buffer_full_up_out[CORE_NUM], buffer_full_up_right[CORE_NUM], buffer_full_up_down[CORE_NUM];

    int in_x_id_in = V_ID_In % Torus_X_NUM;
    int in_y_id_in = (V_ID_In % CORE_NUM) / Torus_X_NUM;
    int in_x_id_left = V_ID_Left % Torus_X_NUM;
    int in_y_id_left = (V_ID_Left % CORE_NUM) / Torus_X_NUM;
    int in_x_id_up = V_ID_Up % Torus_X_NUM;
    int in_y_id_up = (V_ID_Up % CORE_NUM) / Torus_X_NUM;

    buffer_empty_in_out[Torus_Node_ID] = (id_buffer_in_out[Torus_Node_ID].size() == 0);
    buffer_empty_in_right[Torus_Node_ID] = (id_buffer_in_right[Torus_Node_ID].size() == 0);
    buffer_empty_in_down[Torus_Node_ID] = (id_buffer_in_down[Torus_Node_ID].size() == 0);
    buffer_empty_left_out[Torus_Node_ID] = (id_buffer_left_out[Torus_Node_ID].size() == 0);
    buffer_empty_left_right[Torus_Node_ID] = (id_buffer_left_right[Torus_Node_ID].size() == 0);
    buffer_empty_left_down[Torus_Node_ID] = (id_buffer_left_down[Torus_Node_ID].size() == 0);
    buffer_empty_up_out[Torus_Node_ID] = (id_buffer_up_out[Torus_Node_ID].size() == 0);
    buffer_empty_up_right[Torus_Node_ID] = (id_buffer_up_right[Torus_Node_ID].size() == 0);
    buffer_empty_up_down[Torus_Node_ID] = (id_buffer_up_down[Torus_Node_ID].size() == 0);

    if (rst_rd) {
        /*buffer initilization*/
    }
    else {
        if (V_Valid_In) {
            if (in_x_id_in != Torus_Node_ID_X) {
                id_buffer_in_right[Torus_Node_ID].push(V_ID_In);
            }
            else if (in_y_id_in != Torus_Node_ID_Y) {
                id_buffer_in_down[Torus_Node_ID].push(V_ID_In);
            }
            else {
                id_buffer_in_out[Torus_Node_ID].push(V_ID_In);
            }
        }
        if (V_Valid_Left) {
            if (in_x_id_left != Torus_Node_ID_X) {
                id_buffer_left_right[Torus_Node_ID].push(V_ID_Left);
            }
            else if (in_y_id_left != Torus_Node_ID_Y) {
                id_buffer_left_down[Torus_Node_ID].push(V_ID_Left);
            }
            else {
                id_buffer_left_out[Torus_Node_ID].push(V_ID_Left);
            }
        }
        if(V_Valid_Up){
            if (in_x_id_up != Torus_Node_ID_X) {
                id_buffer_up_right[Torus_Node_ID].push(V_ID_Up);
            }
            else if (in_y_id_up != Torus_Node_ID_Y) {
                id_buffer_up_down[Torus_Node_ID].push(V_ID_Up);
            }
            else {
                id_buffer_up_out[Torus_Node_ID].push(V_ID_Up);
            }
        }
    }

    if (rst_rd || Next_Stage_Full1) {
        *To2_V_ID_Out = 0;
        *To2_V_Valid_Out = 0;
    }
    else {
        if (id_buffer_in_out[Torus_Node_ID].size() > id_buffer_left_out[Torus_Node_ID].size() && id_buffer_in_out[Torus_Node_ID].size() > id_buffer_up_out[Torus_Node_ID].size()) { //in1_out1 cannot be empty
            *To2_V_ID_Out = id_buffer_in_out[Torus_Node_ID].front();
            *To2_V_Valid_Out = 1;

            push_flag_buffer_in_out[Torus_Node_ID].pop();
            id_buffer_in_out[Torus_Node_ID].pop();
            value_buffer_in_out[Torus_Node_ID].pop();
            pull_first_flag_buffer_in_out[Torus_Node_ID].pop();
        }
        else if (id_buffer_left_out[Torus_Node_ID].size() > id_buffer_up_out[Torus_Node_ID].size()){
            *To2_V_ID_Out = id_buffer_left_out[Torus_Node_ID].front();
            *To2_V_Valid_Out = 1;

            push_flag_buffer_left_out[Torus_Node_ID].pop();
            id_buffer_left_out[Torus_Node_ID].pop();
            value_buffer_left_out[Torus_Node_ID].pop();
            pull_first_flag_buffer_left_out[Torus_Node_ID].pop();
        }
        else{
            if (buffer_empty_up_out[Torus_Node_ID]) {
                *To2_V_ID_Out = 0;
                *To2_V_Valid_Out = 0;
            }
            else {
                *To2_V_ID_Out = id_buffer_up_out[Torus_Node_ID].front();
                *To2_V_Valid_Out = 1;

                push_flag_buffer_up_out[Torus_Node_ID].pop();
                id_buffer_up_out[Torus_Node_ID].pop();
                value_buffer_up_out[Torus_Node_ID].pop();
                pull_first_flag_buffer_up_out[Torus_Node_ID].pop();
            }
        }
    }

    if (rst_rd || Next_Stage_Full2) {
        *To2_V_ID_Right = 0;
        *To2_V_Valid_Right = 0;
    }
    else {
        if (!buffer_empty_left_right[Torus_Node_ID]) { //in1_right1 cannot be empty
            *To2_V_ID_Right = id_buffer_left_right[Torus_Node_ID].front();
            *To2_V_Valid_Right = 1;

            push_flag_buffer_left_right[Torus_Node_ID].pop();
            id_buffer_left_right[Torus_Node_ID].pop();
            value_buffer_left_right[Torus_Node_ID].pop();
            pull_first_flag_buffer_left_right[Torus_Node_ID].pop();
        }
        else if (!buffer_empty_up_right[Torus_Node_ID]){
            *To2_V_ID_Right = id_buffer_up_right[Torus_Node_ID].front();
            *To2_V_Valid_Right = 1;

            push_flag_buffer_up_right[Torus_Node_ID].pop();
            id_buffer_up_right[Torus_Node_ID].pop();
            value_buffer_up_right[Torus_Node_ID].pop();
            pull_first_flag_buffer_up_right[Torus_Node_ID].pop();
        }
        else{
            if (buffer_empty_in_right[Torus_Node_ID]) {
                *To2_V_ID_Right = 0;
                *To2_V_Valid_Right = 0;
            }
            else {
                *To2_V_ID_Right = id_buffer_in_right[Torus_Node_ID].front();
                *To2_V_Valid_Right = 1;

                push_flag_buffer_in_right[Torus_Node_ID].pop();
                id_buffer_in_right[Torus_Node_ID].pop();
                value_buffer_in_right[Torus_Node_ID].pop();
                pull_first_flag_buffer_in_right[Torus_Node_ID].pop();
            }
        }
    }

    if (rst_rd || Next_Stage_Full3) {
        *To2_V_ID_Down = 0;
        *To2_V_Valid_Down = 0;
    }
    else {
        if (!buffer_empty_up_down[Torus_Node_ID]) { //in1_down1 cannot be empty
            *To2_V_ID_Down = id_buffer_up_down[Torus_Node_ID].front();
            *To2_V_Valid_Down = 1;

            push_flag_buffer_up_down[Torus_Node_ID].pop();
            id_buffer_up_down[Torus_Node_ID].pop();
            value_buffer_up_down[Torus_Node_ID].pop();
            pull_first_flag_buffer_up_down[Torus_Node_ID].pop();
        }
        else if (!buffer_empty_left_down[Torus_Node_ID]){
            *To2_V_ID_Down = id_buffer_left_down[Torus_Node_ID].front();
            *To2_V_Valid_Down = 1;

            push_flag_buffer_left_down[Torus_Node_ID].pop();
            id_buffer_left_down[Torus_Node_ID].pop();
            value_buffer_left_down[Torus_Node_ID].pop();
            pull_first_flag_buffer_left_down[Torus_Node_ID].pop();
        }
        else{
            if (buffer_empty_in_down[Torus_Node_ID]) {
                *To2_V_ID_Down = 0;
                *To2_V_Valid_Down = 0;
            }
            else {
                *To2_V_ID_Down = id_buffer_in_down[Torus_Node_ID].front();
                *To2_V_Valid_Down = 1;

                push_flag_buffer_in_down[Torus_Node_ID].pop();
                id_buffer_in_down[Torus_Node_ID].pop();
                value_buffer_in_down[Torus_Node_ID].pop();
                pull_first_flag_buffer_in_down[Torus_Node_ID].pop();
            }
        }
    }

    buffer_full_in_out[Torus_Node_ID] = (id_buffer_in_out[Torus_Node_ID].size() >= TO_FIFO_SIZE-6);
    buffer_full_in_right[Torus_Node_ID] = (id_buffer_in_right[Torus_Node_ID].size() >= TO_FIFO_SIZE-6);
    buffer_full_in_down[Torus_Node_ID] = (id_buffer_in_down[Torus_Node_ID].size() >= TO_FIFO_SIZE-6);
    buffer_full_left_out[Torus_Node_ID] = (id_buffer_left_out[Torus_Node_ID].size() >= TO_FIFO_SIZE);
    buffer_full_left_right[Torus_Node_ID] = (id_buffer_left_right[Torus_Node_ID].size() >= TO_FIFO_SIZE);
    buffer_full_left_down[Torus_Node_ID] = (id_buffer_left_down[Torus_Node_ID].size() >= TO_FIFO_SIZE);
    buffer_full_up_out[Torus_Node_ID] = (id_buffer_up_out[Torus_Node_ID].size() >= TO_FIFO_SIZE);
    buffer_full_up_right[Torus_Node_ID] = (id_buffer_up_right[Torus_Node_ID].size() >= TO_FIFO_SIZE);
    buffer_full_up_down[Torus_Node_ID] = (id_buffer_up_down[Torus_Node_ID].size() >= TO_FIFO_SIZE);

    *Stage_Full1 = buffer_full_in_out[Torus_Node_ID] || buffer_full_in_right[Torus_Node_ID] || buffer_full_in_down[Torus_Node_ID];
    *Stage_Full2 = buffer_full_left_out[Torus_Node_ID] || buffer_full_left_right[Torus_Node_ID] || buffer_full_left_down[Torus_Node_ID];
    *Stage_Full3 = buffer_full_up_out[Torus_Node_ID] || buffer_full_up_right[Torus_Node_ID] || buffer_full_up_down[Torus_Node_ID];
}