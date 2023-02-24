//
// Created by test on 2023/2/23.
//

#include "ACC.h"


extern int clk;
extern int rst_rd;

using namespace std;

//map映射记录进入和写出的点 对应观察是否存在重复写出和丢失的点

static int mp_hy1[1048576];
static int mp_hy2[1048576];
static int rst_hy1 = 0;
static int rst_hy2 = 0;

static int mp_global_in;
static int mp_global_out;
/*write in pipe*/
static int tmp_hy1_vertical_push_flag_in[CORE_NUM][VERTICAL_LEN], tmp_hy1_vertical_v_id_in[CORE_NUM][VERTICAL_LEN],
        tmp_hy1_vertical_v_value_in[CORE_NUM][VERTICAL_LEN],tmp_hy1_vertical_pull_first_flag_in[CORE_NUM][VERTICAL_LEN],
        tmp_hy1_vertical_v_valid_in[CORE_NUM][VERTICAL_LEN], tmp_hy1_vertical_stage_full[CORE_NUM][VERTICAL_LEN],
        tmp_hy1_vertical_block[CORE_NUM][VERTICAL_LEN];
static int tmp_hy1_horizontal_push_flag_in[CORE_NUM][HORIZONTAL_LEN], tmp_hy1_horizontal_v_id_in[CORE_NUM][HORIZONTAL_LEN], tmp_hy1_horizontal_v_value_in[CORE_NUM][HORIZONTAL_LEN],
        tmp_hy1_horizontal_pull_first_flag_in[CORE_NUM][HORIZONTAL_LEN], tmp_hy1_horizontal_v_valid_in[CORE_NUM][HORIZONTAL_LEN],  tmp_hy1_horizontal_stage_full[CORE_NUM][HORIZONTAL_LEN], tmp_hy1_horizontal_block[CORE_NUM][HORIZONTAL_LEN];


static int tmp_hy2_vertical_push_flag_in[CORE_NUM][VERTICAL_LEN], tmp_hy2_vertical_v_id_in[CORE_NUM][VERTICAL_LEN], tmp_hy2_vertical_v_value_in[CORE_NUM][VERTICAL_LEN],
        tmp_hy2_vertical_pull_first_flag_in[CORE_NUM][VERTICAL_LEN], tmp_hy2_vertical_v_valid_in[CORE_NUM][VERTICAL_LEN],  tmp_hy2_vertical_stage_full[CORE_NUM][VERTICAL_LEN], tmp_hy2_vertical_block[CORE_NUM][VERTICAL_LEN];
static int tmp_hy2_horizontal_push_flag_in[CORE_NUM][HORIZONTAL_LEN], tmp_hy2_horizontal_v_id_in[CORE_NUM][HORIZONTAL_LEN], tmp_hy2_horizontal_v_value_in[CORE_NUM][HORIZONTAL_LEN],
        tmp_hy2_horizontal_pull_first_flag_in[CORE_NUM][HORIZONTAL_LEN], tmp_hy2_horizontal_v_valid_in[CORE_NUM][HORIZONTAL_LEN],  tmp_hy2_horizontal_stage_full[CORE_NUM][HORIZONTAL_LEN], tmp_hy2_horizontal_block[CORE_NUM][HORIZONTAL_LEN];
/*write in pipe*/

/*schedule*/
static int hy1_input_write_vertical_buffer[CORE_NUM],hy1_input_write_horizontal_buffer[CORE_NUM],hy1_input_write_output[CORE_NUM],
        hy1_horizontal_write_vertical_buffer[CORE_NUM],hy1_horizontal_write_output[CORE_NUM],
        hy1_vertical_write_output[CORE_NUM];

static int hy2_input_write_vertical_buffer[CORE_NUM],hy2_input_write_horizontal_buffer[CORE_NUM],hy2_input_write_output[CORE_NUM],
        hy2_horizontal_write_vertical_buffer[CORE_NUM],hy2_horizontal_write_output[CORE_NUM],
        hy2_vertical_write_output[CORE_NUM];
/*schedule*/
/*read pipe schedule pass to the hyperx*/
static int tmp_hy1_horizontal_v_valid_index_min[CORE_NUM] ,tmp_hy1_horizontal_v_valid_in_min[CORE_NUM],tmp_hy1_vertical_v_valid_index_min[CORE_NUM] ,tmp_hy1_vertical_v_valid_in_min[CORE_NUM],
        tmp_hy2_horizontal_v_valid_index_min[CORE_NUM] ,tmp_hy2_horizontal_v_valid_in_min[CORE_NUM],tmp_hy2_vertical_v_valid_index_min[CORE_NUM] ,tmp_hy2_vertical_v_valid_in_min[CORE_NUM];
/*read pipe schedule*/

/*read pipe */
static int hy1_vertical_block[CORE_NUM][VERTICAL_LEN], hy1_vertical_stage_full[CORE_NUM][VERTICAL_LEN], hy1_vertical_push_flag_in[CORE_NUM][VERTICAL_LEN],
        hy1_vertical_v_id_in[CORE_NUM][VERTICAL_LEN], hy1_vertical_v_value_in[CORE_NUM][VERTICAL_LEN], hy1_vertical_pull_first_flag_in[CORE_NUM][VERTICAL_LEN],
        hy1_vertical_v_valid_in[CORE_NUM][VERTICAL_LEN];
static int hy1_horizontal_block[CORE_NUM][HORIZONTAL_LEN], hy1_horizontal_stage_full[CORE_NUM][HORIZONTAL_LEN], hy1_horizontal_push_flag_in[CORE_NUM][HORIZONTAL_LEN],
        hy1_horizontal_v_id_in[CORE_NUM][HORIZONTAL_LEN], hy1_horizontal_v_value_in[CORE_NUM][HORIZONTAL_LEN], hy1_horizontal_pull_first_flag_in[CORE_NUM][HORIZONTAL_LEN],
        hy1_horizontal_v_valid_in[CORE_NUM][HORIZONTAL_LEN];

static int hy2_vertical_block[CORE_NUM][VERTICAL_LEN], hy2_vertical_stage_full[CORE_NUM][VERTICAL_LEN], hy2_vertical_push_flag_in[CORE_NUM][VERTICAL_LEN],
        hy2_vertical_v_id_in[CORE_NUM][VERTICAL_LEN], hy2_vertical_v_value_in[CORE_NUM][VERTICAL_LEN], hy2_vertical_pull_first_flag_in[CORE_NUM][VERTICAL_LEN],
        hy2_vertical_v_valid_in[CORE_NUM][VERTICAL_LEN];
static int hy2_horizontal_block[CORE_NUM][HORIZONTAL_LEN], hy2_horizontal_stage_full[CORE_NUM][HORIZONTAL_LEN], hy2_horizontal_push_flag_in[CORE_NUM][HORIZONTAL_LEN],
        hy2_horizontal_v_id_in[CORE_NUM][HORIZONTAL_LEN], hy2_horizontal_v_value_in[CORE_NUM][HORIZONTAL_LEN], hy2_horizontal_pull_first_flag_in[CORE_NUM][HORIZONTAL_LEN],
        hy2_horizontal_v_valid_in[CORE_NUM][HORIZONTAL_LEN];
/*read pipe*/

//input horizontal target col,row
static int tmp_hy1_input_col_id[CORE_NUM], tmp_hy1_input_row_id[CORE_NUM], tmp_hy1_horizontal_col_id[CORE_NUM], tmp_hy1_horizontal_row_id[CORE_NUM];
static int tmp_hy2_input_col_id[CORE_NUM], tmp_hy2_input_row_id[CORE_NUM], tmp_hy2_horizontal_col_id[CORE_NUM], tmp_hy2_horizontal_row_id[CORE_NUM];

/*write old schedule sign*/
static int hy1_write_old_vertical_network[CORE_NUM], hy2_write_old_vertical_network[CORE_NUM];
static int hy1_write_old_vertical_network_block[CORE_NUM], hy2_write_old_vertical_network_block[CORE_NUM];

void Hyperx_Network_2_Single(int Hyperx_Switch_ID,
                             int Push_Flag, int V_ID_In, int V_Value_In, int Pull_First_Flag, int V_Valid_In,
                             int Iteration_End_In, int Iteration_End_DValid_In,int Next_Stage_Full,


                             int *Hy_Vertical_Push_Flag_Out, int *Hy_Vertical_V_ID_Out, int *Hy_Vertical_V_Value_Out, int *Hy_Vertical_Pull_First_Flag,
                             int *Hy_Vertical_V_Valid_Out,
                             int *Vertical_Stage_Full,int* Vertical_Block,

                             int *Hy_Horizontal_Push_Flag_Out, int *Hy_Horizontal_V_ID_Out, int *Hy_Horizontal_V_Value_Out, int *Hy_Horizontal_Pull_First_Flag,
                             int *Hy_Horizontal_V_Valid_Out,
                             int *Horizontal_Stage_Full,int *Horizontal_Block,

                             int *Hy_Update_V_ID, int *Hy_Update_V_DValid,
                             int *Stage_Full_Hy,

                             int* combine_input_buffer_size,int *combine_push_flag_buffer_input_front,int *combine_id_buffer_input_front,int *combine_value_buffer_input_front,int * combine_pull_first_flag_buffer_input_front,
                             int* combine_horizontal_buffer_size,int *combine_push_flag_buffer_hmp_hy1orizontal_front,int *combine_id_buffer_horizontal_front,int *combine_value_buffer_horizontal_front,int * combine_pull_first_flag_buffer_horizontal_front,
                             int* combine_vertical_buffer_size,int *combine_push_flag_buffer_vertical_front,int *combine_id_buffer_vertical_front,int *combine_value_buffer_vertical_front,int * combine_pull_first_flag_buffer_vertical_front);

void Hyperx_Network(int Hy2_Front_Update_V_ID[], int Hy2_Front_Update_V_DValid[],
                    int Source_Core_Full[],

                    int *Stage_Full_Hy2,
                    int *Hy2_Update_V_ID, int *Hy2_Update_V_DValid) {
//硬件排序上彼此间连线更简单的一种实现方法
    static int tmp_hy1_push_flag_in[CORE_NUM], tmp_hy1_v_id_in[CORE_NUM], tmp_hy1_v_value_in[CORE_NUM], tmp_hy1_pull_first_flag_in[CORE_NUM], tmp_hy1_v_valid_in[CORE_NUM],
            tmp_hy1_iteration_end[CORE_NUM], tmp_hy1_iteration_end_dvalid[CORE_NUM], tmp_hy1_stage_full[CORE_NUM];

    static int tmp_hy2_push_flag_in[CORE_NUM], tmp_hy2_v_id_in[CORE_NUM], tmp_hy2_v_value_in[CORE_NUM], tmp_hy2_pull_first_flag_in[CORE_NUM], tmp_hy2_v_valid_in[CORE_NUM],
            tmp_hy2_iteration_end[CORE_NUM], tmp_hy2_iteration_end_dvalid[CORE_NUM], tmp_hy2_stage_full[CORE_NUM];

    /*read pipe schedule*/
    int tmp_hy1_horizontal_v_valid_in_arr[CORE_NUM][HORIZONTAL_LEN], tmp_hy1_horizontal_v_valid_index_arr[CORE_NUM][HORIZONTAL_LEN];
    int tmp_hy1_vertical_v_valid_in_arr[CORE_NUM][VERTICAL_LEN], tmp_hy1_vertical_v_valid_index_arr[CORE_NUM][VERTICAL_LEN];
    int tmp_hy2_horizontal_v_valid_in_arr[CORE_NUM][HORIZONTAL_LEN], tmp_hy2_horizontal_v_valid_index_arr[CORE_NUM][HORIZONTAL_LEN];
    int tmp_hy2_vertical_v_valid_in_arr[CORE_NUM][VERTICAL_LEN], tmp_hy2_vertical_v_valid_index_arr[CORE_NUM][VERTICAL_LEN];
    /*read pipe schedule*/

    /*combine logic */
    static int hy1_combine_horizontal_buffer_size[CORE_NUM],hy1_combine_push_flag_buffer_horizontal_front[CORE_NUM],hy1_combine_id_buffer_horizontal_front[CORE_NUM],hy1_combine_value_buffer_horizontal_front[CORE_NUM],hy1_combine_pull_first_flag_buffer_horizontal_front[CORE_NUM],
            hy1_combine_vertical_buffer_size[CORE_NUM],hy1_combine_push_flag_buffer_vertical_front[CORE_NUM],hy1_combine_id_buffer_vertical_front[CORE_NUM],hy1_combine_value_buffer_vertical_front[CORE_NUM],hy1_combine_pull_first_flag_buffer_vertical_front[CORE_NUM],
            hy1_combine_input_buffer_size[CORE_NUM],hy1_combine_push_flag_buffer_input_front[CORE_NUM],hy1_combine_id_buffer_input_front[CORE_NUM],hy1_combine_value_buffer_input_front[CORE_NUM],hy1_combine_pull_first_flag_buffer_input_front[CORE_NUM];
    static int hy2_combine_horizontal_buffer_size[CORE_NUM],hy2_combine_push_flag_buffer_horizontal_front[CORE_NUM],hy2_combine_id_buffer_horizontal_front[CORE_NUM],hy2_combine_value_buffer_horizontal_front[CORE_NUM],hy2_combine_pull_first_flag_buffer_horizontal_front[CORE_NUM],
            hy2_combine_vertical_buffer_size[CORE_NUM],hy2_combine_push_flag_buffer_vertical_front[CORE_NUM],hy2_combine_id_buffer_vertical_front[CORE_NUM],hy2_combine_value_buffer_vertical_front[CORE_NUM],hy2_combine_pull_first_flag_buffer_vertical_front[CORE_NUM],
            hy2_combine_input_buffer_size[CORE_NUM],hy2_combine_push_flag_buffer_input_front[CORE_NUM],hy2_combine_id_buffer_input_front[CORE_NUM],hy2_combine_value_buffer_input_front[CORE_NUM],hy2_combine_pull_first_flag_buffer_input_front[CORE_NUM];

    /*combine logic */

    for (int i = 0; i < CORE_NUM; ++ i) {
        tmp_hy2_v_id_in[i] = Hy2_Front_Update_V_ID[i];
        tmp_hy2_v_valid_in[i] = Hy2_Front_Update_V_DValid[i];
        tmp_hy2_stage_full[i] = Source_Core_Full[i]; //destination core never full
    }
    //send info
    for (int i = 0; i < CORE_NUM; ++ i) {
        for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
            hy2_horizontal_block[i][j] = tmp_hy2_horizontal_block[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
        }

        for(int j = 0 ; j<VERTICAL_LEN; j++){
            hy2_vertical_block[i][j] = tmp_hy2_vertical_block[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
        }

    }

    for (int i = 1; i <= CORE_NUM; i++) {
        Hyperx_Network_2_Single(i-1 ,
                                tmp_hy2_push_flag_in[i - 1], tmp_hy2_v_id_in[i - 1], tmp_hy2_v_value_in[i - 1], tmp_hy2_pull_first_flag_in[i - 1],
                                tmp_hy2_v_valid_in[i - 1], tmp_hy2_iteration_end[i - 1], tmp_hy2_iteration_end_dvalid[i - 1],tmp_hy2_stage_full[i-1],
                /*read*/

                /*write*/
                                tmp_hy2_vertical_push_flag_in[i-1], tmp_hy2_vertical_v_id_in[i-1], tmp_hy2_vertical_v_value_in[i-1],
                                tmp_hy2_vertical_pull_first_flag_in[i-1], tmp_hy2_vertical_v_valid_in[i-1], tmp_hy2_vertical_stage_full[i-1],tmp_hy2_vertical_block[i-1],

                                tmp_hy2_horizontal_push_flag_in[i-1], tmp_hy2_horizontal_v_id_in[i-1], tmp_hy2_horizontal_v_value_in[i-1],
                                tmp_hy2_horizontal_pull_first_flag_in[i-1], tmp_hy2_horizontal_v_valid_in[i-1], tmp_hy2_horizontal_stage_full[i-1],tmp_hy2_horizontal_block[i-1],

                                Hy2_Update_V_ID, Hy2_Update_V_DValid,
                                Stage_Full_Hy2,

                                &hy2_combine_input_buffer_size[i-1],&hy2_combine_push_flag_buffer_input_front[i-1],&hy2_combine_id_buffer_input_front[i-1],&hy2_combine_value_buffer_input_front[i-1],&hy2_combine_pull_first_flag_buffer_input_front[i-1] ,
                                &hy2_combine_horizontal_buffer_size[i-1],&hy2_combine_push_flag_buffer_horizontal_front[i-1],&hy2_combine_id_buffer_horizontal_front[i-1],&hy2_combine_value_buffer_horizontal_front[i-1], &hy2_combine_pull_first_flag_buffer_horizontal_front[i-1] ,
                                &hy2_combine_vertical_buffer_size[i-1],&hy2_combine_push_flag_buffer_vertical_front[i-1],&hy2_combine_id_buffer_vertical_front[i-1],&hy2_combine_value_buffer_vertical_front[i-1], &hy2_combine_pull_first_flag_buffer_vertical_front[i-1]
        );
    }


    for(int i = 0 ; i<CORE_NUM ; i++){
        int tmp_hy2_horizontal_v_id_front = hy2_combine_id_buffer_horizontal_front[i];
        tmp_hy2_horizontal_row_id[i] = (tmp_hy2_horizontal_v_id_front%CORE_NUM)/HORIZONTAL_LEN;
        tmp_hy2_horizontal_col_id[i] = (tmp_hy2_horizontal_v_id_front%CORE_NUM)%HORIZONTAL_LEN;
        int tmp_hy2_input_v_id_front = hy2_combine_id_buffer_input_front[i];
        tmp_hy2_input_row_id[i] = (tmp_hy2_input_v_id_front%CORE_NUM)/HORIZONTAL_LEN;
        tmp_hy2_input_col_id[i] = (tmp_hy2_input_v_id_front%CORE_NUM)%HORIZONTAL_LEN;
    }

    //send dtage full info
    for (int i = 0; i < CORE_NUM; ++ i) {
        for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
            hy2_horizontal_stage_full[i][j] = tmp_hy2_horizontal_stage_full[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
        }

        for(int j = 0 ; j<VERTICAL_LEN; j++){
            hy2_vertical_stage_full[i][j] = tmp_hy2_vertical_stage_full[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
        }
    }

    /*write output combine logic*/

    for(int i = 0 ; i<CORE_NUM ; i++){
        if(hy2_combine_vertical_buffer_size[i]){
            hy2_vertical_write_output[i] = 1;
            hy2_horizontal_write_output[i] = 0;
            hy2_input_write_output[i] = 0;
        }else if(hy2_combine_horizontal_buffer_size[i] && tmp_hy2_horizontal_col_id[i] == i%HORIZONTAL_LEN && tmp_hy2_horizontal_row_id[i] == i/HORIZONTAL_LEN){
            hy2_vertical_write_output[i] = 0;
            hy2_horizontal_write_output[i] = 1;
            hy2_input_write_output[i] = 0;
        }else if(hy2_combine_input_buffer_size[i] &&  tmp_hy2_input_col_id[i] == i%HORIZONTAL_LEN && tmp_hy2_input_row_id[i] == i/HORIZONTAL_LEN){
            hy2_vertical_write_output[i] = 0;
            hy2_horizontal_write_output[i] = 0;
            hy2_input_write_output[i] = 1;
        }else{
            hy2_vertical_write_output[i] = 0;
            hy2_horizontal_write_output[i] = 0;
            hy2_input_write_output[i] = 0;
        }
    }

//input horizontal write vertical
    for(int i = 0 ; i<CORE_NUM ; ++ i){
        int tmp_hy2_input_write_vertical = 0;
        int tmp_hy2_horizontal_write_vertical = 0;
        if(hy2_combine_horizontal_buffer_size[i] && !hy2_horizontal_write_output[i] && tmp_hy2_horizontal_row_id[i] != i/HORIZONTAL_LEN){
            tmp_hy2_horizontal_write_vertical = 1;
        }else{
            tmp_hy2_horizontal_write_vertical = 0;
        }
        if(hy2_combine_input_buffer_size[i] && !hy2_input_write_output[i] && tmp_hy2_input_row_id[i] != i/HORIZONTAL_LEN && tmp_hy2_input_col_id[i] == i%HORIZONTAL_LEN){
            tmp_hy2_input_write_vertical = 1;
            //printf("id:%d input write vertical buffer\n",tmp_hy2_input_v_id_front);
        }else{
            tmp_hy2_input_write_vertical = 0;
        }

        if( tmp_hy2_horizontal_write_vertical && tmp_hy2_input_write_vertical && tmp_hy2_input_row_id[i] == tmp_hy2_horizontal_row_id[i]){
            if((hy2_write_old_vertical_network[i] == 1 || hy2_write_old_vertical_network[i] == 3) && hy2_write_old_vertical_network_block[i] ==1){
                hy2_horizontal_write_vertical_buffer[i] = 1;
                hy2_input_write_vertical_buffer[i] = 0;
            }else if((hy2_write_old_vertical_network[i] == 2 || hy2_write_old_vertical_network[i] == 3) && hy2_write_old_vertical_network_block[i] ==2){
                hy2_horizontal_write_vertical_buffer[i] = 0;
                hy2_input_write_vertical_buffer[i] = 1;
            }else{
                hy2_input_write_vertical_buffer[i] = 0;
                hy2_horizontal_write_vertical_buffer[i] = 1;
            }
        }else{
            hy2_input_write_vertical_buffer[i] = tmp_hy2_input_write_vertical;
            hy2_horizontal_write_vertical_buffer[i] = tmp_hy2_horizontal_write_vertical;
        }

        for(int j = 0 ; j<VERTICAL_LEN ; j++){
            if( j == tmp_hy2_horizontal_row_id[i] && hy2_horizontal_write_vertical_buffer[i]){
                tmp_hy2_vertical_push_flag_in[i][j] = hy2_combine_push_flag_buffer_horizontal_front[i];
                tmp_hy2_vertical_v_id_in[i][j] = hy2_combine_id_buffer_horizontal_front[i];
                tmp_hy2_vertical_pull_first_flag_in[i][j] = hy2_combine_pull_first_flag_buffer_horizontal_front[i];
                tmp_hy2_vertical_v_value_in[i][j] = hy2_combine_value_buffer_horizontal_front[i];
                tmp_hy2_vertical_v_valid_in[i][j] = !hy2_vertical_stage_full[i][tmp_hy2_horizontal_row_id[i]];
            }else if( j == tmp_hy2_input_row_id[i] && hy2_input_write_vertical_buffer[i]){
                tmp_hy2_vertical_push_flag_in[i][j] = hy2_combine_push_flag_buffer_input_front[i];
                tmp_hy2_vertical_v_id_in[i][j] = hy2_combine_id_buffer_input_front[i];
                tmp_hy2_vertical_pull_first_flag_in[i][j] = hy2_combine_pull_first_flag_buffer_input_front[i];
                tmp_hy2_vertical_v_value_in[i][j] = hy2_combine_value_buffer_input_front[i];
                tmp_hy2_vertical_v_valid_in[i][j] = !hy2_vertical_stage_full[i][tmp_hy2_input_row_id[i]];
            }else{
                tmp_hy2_vertical_push_flag_in[i][j] = 0;
                tmp_hy2_vertical_v_id_in[i][j] = 0;
                tmp_hy2_vertical_pull_first_flag_in[i][j] = 0;
                tmp_hy2_vertical_v_value_in[i][j] = 0;
                tmp_hy2_vertical_v_valid_in[i][j] = 0;
            }
        }
    }

    /* write horizontal buffer combine logic */
    for(int i = 0 ; i<CORE_NUM ; i++){
        if(hy2_combine_input_buffer_size[i] && !hy2_input_write_vertical_buffer[i] && !hy2_input_write_output[i] && tmp_hy2_input_col_id[i] != i%HORIZONTAL_LEN){
            hy2_input_write_horizontal_buffer[i] = 1;
        }else{
            hy2_input_write_horizontal_buffer[i] = 0;
        }
    }


    //input write horizontal
    for(int i = 0 ; i<CORE_NUM ; ++ i){
        for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
            if(j == tmp_hy2_input_col_id[i] && hy2_input_write_horizontal_buffer[i]){
                tmp_hy2_horizontal_push_flag_in[i][j] = hy2_combine_push_flag_buffer_input_front[i];
                tmp_hy2_horizontal_v_id_in[i][j] = hy2_combine_id_buffer_input_front[i];
                tmp_hy2_horizontal_pull_first_flag_in[i][j] = hy2_combine_pull_first_flag_buffer_input_front[i];
                tmp_hy2_horizontal_v_value_in[i][j] = hy2_combine_value_buffer_input_front[i];
                tmp_hy2_horizontal_v_valid_in[i][j] = (!hy2_horizontal_stage_full[i][tmp_hy2_input_col_id[i]])? 1 : 0;
            }else{
                tmp_hy2_horizontal_push_flag_in[i][j] = 0;
                tmp_hy2_horizontal_v_id_in[i][j] = 0;
                tmp_hy2_horizontal_pull_first_flag_in[i][j] = 0;
                tmp_hy2_horizontal_v_value_in[i][j] = 0;
                tmp_hy2_horizontal_v_valid_in[i][j] = 0;
            }
        }
    }

    //send info
    for (int i = 0; i < CORE_NUM; ++ i) {
        for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
            hy2_horizontal_push_flag_in[i][j] = tmp_hy2_horizontal_push_flag_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
            hy2_horizontal_v_id_in[i][j] = tmp_hy2_horizontal_v_id_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
            hy2_horizontal_v_value_in[i][j] = tmp_hy2_horizontal_v_value_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
            hy2_horizontal_pull_first_flag_in[i][j] = tmp_hy2_horizontal_pull_first_flag_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
            hy2_horizontal_v_valid_in[i][j] = tmp_hy2_horizontal_v_valid_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
        }

        for(int j = 0 ; j<VERTICAL_LEN; j++){
            hy2_vertical_push_flag_in[i][j] = tmp_hy2_vertical_push_flag_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
            hy2_vertical_v_id_in[i][j] = tmp_hy2_vertical_v_id_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
            hy2_vertical_v_value_in[i][j] = tmp_hy2_vertical_v_value_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
            hy2_vertical_pull_first_flag_in[i][j] = tmp_hy2_vertical_pull_first_flag_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
            hy2_vertical_v_valid_in[i][j] = tmp_hy2_vertical_v_valid_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
        }

    }

    /*vertical buffer receive*/

    /* horizontal buffer receive */
    for(int i = 0 ; i<CORE_NUM ; i++){
        for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
            tmp_hy2_horizontal_v_valid_in_arr[i][j] = tmp_hy2_horizontal_v_valid_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN];
            tmp_hy2_horizontal_v_valid_index_arr[i][j] = j;
        }
        int t = HORIZONTAL_LEN;
        while(t>1){
            t = t/2;
            for(int k = 0 ; k<t ; k++){
                tmp_hy2_horizontal_v_valid_index_arr[i][k] = (tmp_hy2_horizontal_v_valid_in_arr[i][2 * k]) ? tmp_hy2_horizontal_v_valid_index_arr[i][2 * k] : tmp_hy2_horizontal_v_valid_index_arr[i][2 * k + 1];//未被访问则往前传，优先传index小的
                tmp_hy2_horizontal_v_valid_in_arr[i][k] = tmp_hy2_horizontal_v_valid_in_arr[i][2 * k] | tmp_hy2_horizontal_v_valid_in_arr[i][2 * k + 1];
            }
        }
        tmp_hy2_horizontal_v_valid_index_min[i] = tmp_hy2_horizontal_v_valid_index_arr[i][0];
        tmp_hy2_horizontal_v_valid_in_min[i] = tmp_hy2_horizontal_v_valid_in_arr[i][0];
        if (tmp_hy2_horizontal_v_valid_in_min[i]) {
            for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
                if(j == tmp_hy2_horizontal_v_valid_index_min[i] || !tmp_hy2_horizontal_v_valid_in[j+i/HORIZONTAL_LEN*HORIZONTAL_LEN][i%HORIZONTAL_LEN]){
                    tmp_hy2_horizontal_block[i][j] = 0;
                }else{
                    tmp_hy2_horizontal_block[i][j] = 1;
                }
            }
        }else{
            for(int j = 0 ; j<HORIZONTAL_LEN ; j++){
                tmp_hy2_horizontal_block[i][j] = 0;
            }
        }

        for(int j = 0 ; j<VERTICAL_LEN ; j++){
            tmp_hy2_vertical_v_valid_in_arr[i][j] = tmp_hy2_vertical_v_valid_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN];
            tmp_hy2_vertical_v_valid_index_arr[i][j] = j;
        }

        t = VERTICAL_LEN;
        while(t>1){
            t = t/2;
            for(int k = 0 ; k<t ; k++){
                tmp_hy2_vertical_v_valid_index_arr[i][k] = (tmp_hy2_vertical_v_valid_in_arr[i][2 * k]) ? tmp_hy2_vertical_v_valid_index_arr[i][2 * k] : tmp_hy2_vertical_v_valid_index_arr[i][2 * k + 1];//未被访问则往前传，优先传index小的
                tmp_hy2_vertical_v_valid_in_arr[i][k] = tmp_hy2_vertical_v_valid_in_arr[i][2 * k] | tmp_hy2_vertical_v_valid_in_arr[i][2 * k + 1];
            }
        }
        tmp_hy2_vertical_v_valid_index_min[i] = tmp_hy2_vertical_v_valid_index_arr[i][0];
        tmp_hy2_vertical_v_valid_in_min[i] = tmp_hy2_vertical_v_valid_in_arr[i][0];
        if (tmp_hy2_vertical_v_valid_in_min[i]) {
            for(int j = 0 ; j<VERTICAL_LEN ; j++){
                if(j == tmp_hy2_vertical_v_valid_index_min[i] || !tmp_hy2_vertical_v_valid_in[j*HORIZONTAL_LEN+i%HORIZONTAL_LEN][i/HORIZONTAL_LEN]){
                    tmp_hy2_vertical_block[i][j] = 0;
                }else{
                    tmp_hy2_vertical_block[i][j] = 1;
                }
            }
        }else{
            for(int j = 0 ; j<VERTICAL_LEN ; j++){
                tmp_hy2_vertical_block[i][j] = 0;
            }
        }

    }

}

void Hyperx_Network_2_Single(int Hyperx_Switch_ID,
                             int Push_Flag, int V_ID_In, int V_Value_In, int Pull_First_Flag, int V_Valid_In,
                             int Iteration_End_In, int Iteration_End_DValid_In,int Next_Stage_Full,

                             int *Hy_Vertical_Push_Flag_Out, int *Hy_Vertical_V_ID_Out, int *Hy_Vertical_V_Value_Out, int *Hy_Vertical_Pull_First_Flag,
                             int *Hy_Vertical_V_Valid_Out,
                             int *Vertical_Stage_Full,int* Vertical_Block,

                             int *Hy_Horizontal_Push_Flag_Out, int *Hy_Horizontal_V_ID_Out, int *Hy_Horizontal_V_Value_Out, int *Hy_Horizontal_Pull_First_Flag,
                             int *Hy_Horizontal_V_Valid_Out,
                             int *Horizontal_Stage_Full,int *Horizontal_Block,

                             int *Hy_Update_V_ID, int *Hy_Update_V_DValid,
                             int *Stage_Full_Hy,

                             int* combine_input_buffer_size,int *combine_push_flag_buffer_input_front,int *combine_id_buffer_input_front,int *combine_value_buffer_input_front, int * combine_pull_first_flag_buffer_input_front,
                             int* combine_horizontal_buffer_size,int *combine_push_flag_buffer_horizontal_front,int *combine_id_buffer_horizontal_front,int *combine_value_buffer_horizontal_front,int * combine_pull_first_flag_buffer_horizontal_front,
                             int* combine_vertical_buffer_size,int *combine_push_flag_buffer_vertical_front,int *combine_id_buffer_vertical_front,int *combine_value_buffer_vertical_front,int * combine_pull_first_flag_buffer_vertical_front){


    static queue<int> push_flag_buffer_vertical[CORE_NUM], id_buffer_vertical[CORE_NUM], value_buffer_vertical[CORE_NUM], pull_first_flag_buffer_vertical[CORE_NUM];
    static queue<int> push_flag_buffer_horizontal[CORE_NUM], id_buffer_horizontal[CORE_NUM], value_buffer_horizontal[CORE_NUM], pull_first_flag_buffer_horizontal[CORE_NUM];
    static queue<int> push_flag_buffer_input[CORE_NUM], id_buffer_input[CORE_NUM], value_buffer_input[CORE_NUM], pull_first_flag_buffer_input[CORE_NUM];
    //static int iteration_end_clk1[CORE_NUM], iteration_end_dvalid_clk1[CORE_NUM];
    static int vertical_buffer_empty[CORE_NUM], horizontal_buffer_empty[CORE_NUM], input_buffer_empty[CORE_NUM];
    static int vertical_buffer_full[CORE_NUM], horizontal_buffer_full[CORE_NUM], input_buffer_full[CORE_NUM];


    vertical_buffer_empty[Hyperx_Switch_ID] = (id_buffer_vertical[Hyperx_Switch_ID].size() == 0);
    horizontal_buffer_empty[Hyperx_Switch_ID] = (id_buffer_horizontal[Hyperx_Switch_ID].size() == 0);
    input_buffer_empty[Hyperx_Switch_ID] = (id_buffer_input[Hyperx_Switch_ID].size() == 0);

    /*input horizontal_mov*/
    if (rst_rd) {
    }else {
        if(!hy2_horizontal_stage_full[Hyperx_Switch_ID][tmp_hy2_input_col_id[Hyperx_Switch_ID]] && hy2_input_write_horizontal_buffer[Hyperx_Switch_ID] && !hy2_horizontal_block[Hyperx_Switch_ID][tmp_hy2_input_col_id[Hyperx_Switch_ID]]){
            push_flag_buffer_input[Hyperx_Switch_ID].pop();
            id_buffer_input[Hyperx_Switch_ID].pop();
            value_buffer_input[Hyperx_Switch_ID].pop();
            pull_first_flag_buffer_input[Hyperx_Switch_ID].pop();
        }

    }

    /*input/horizontal vertical_mov*/
    if (rst_rd) {
    }else {
        if(!hy2_vertical_stage_full[Hyperx_Switch_ID][tmp_hy2_horizontal_row_id[Hyperx_Switch_ID]] && hy2_horizontal_write_vertical_buffer[Hyperx_Switch_ID] && !hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_horizontal_row_id[Hyperx_Switch_ID]]){
            push_flag_buffer_horizontal[Hyperx_Switch_ID].pop();
            id_buffer_horizontal[Hyperx_Switch_ID].pop();
            value_buffer_horizontal[Hyperx_Switch_ID].pop();
            pull_first_flag_buffer_horizontal[Hyperx_Switch_ID].pop();
        }

        if(!hy2_vertical_stage_full[Hyperx_Switch_ID][tmp_hy2_input_row_id[Hyperx_Switch_ID]] && hy2_input_write_vertical_buffer[Hyperx_Switch_ID] && !hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_input_row_id[Hyperx_Switch_ID]]){
            push_flag_buffer_input[Hyperx_Switch_ID].pop();
            id_buffer_input[Hyperx_Switch_ID].pop();
            value_buffer_input[Hyperx_Switch_ID].pop();
            pull_first_flag_buffer_input[Hyperx_Switch_ID].pop();
        }
    }

    if(rst_rd){

    }else{
        hy2_write_old_vertical_network[Hyperx_Switch_ID] =  (hy2_horizontal_write_vertical_buffer[Hyperx_Switch_ID] && hy2_input_write_vertical_buffer[Hyperx_Switch_ID])?3:(hy2_horizontal_write_vertical_buffer[Hyperx_Switch_ID] ? 1 : (hy2_input_write_vertical_buffer[Hyperx_Switch_ID] ? 2 : 0)) ;
        if(hy2_horizontal_write_vertical_buffer[Hyperx_Switch_ID] && hy2_input_write_vertical_buffer[Hyperx_Switch_ID]){
            hy2_write_old_vertical_network_block[Hyperx_Switch_ID] = hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_horizontal_row_id[Hyperx_Switch_ID]] && hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_input_row_id[Hyperx_Switch_ID]] ? 3 : (hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_horizontal_row_id[Hyperx_Switch_ID]] ? 1 : ( hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_input_row_id[Hyperx_Switch_ID]] ? 2 : 0));
        }else if(hy2_horizontal_write_vertical_buffer[Hyperx_Switch_ID]){
            hy2_write_old_vertical_network_block[Hyperx_Switch_ID] = hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_horizontal_row_id[Hyperx_Switch_ID]] ? 1 : 0;
        }else if(hy2_input_write_vertical_buffer[Hyperx_Switch_ID]){
            hy2_write_old_vertical_network_block[Hyperx_Switch_ID] = hy2_vertical_block[Hyperx_Switch_ID][tmp_hy2_input_row_id[Hyperx_Switch_ID]] ? 2 : 0;
        }else{
            hy2_write_old_vertical_network_block[Hyperx_Switch_ID] = 0;
        }
    }

    /*input/horizontal/vertical output_mov*/
    if(Next_Stage_Full){
        Hy_Update_V_ID[Hyperx_Switch_ID] = 0;
        Hy_Update_V_DValid[Hyperx_Switch_ID] = 0;
    }else{
        if(hy2_vertical_write_output[Hyperx_Switch_ID]){
            Hy_Update_V_ID[Hyperx_Switch_ID] = id_buffer_vertical[Hyperx_Switch_ID].front();
            Hy_Update_V_DValid[Hyperx_Switch_ID] = 1;

            push_flag_buffer_vertical[Hyperx_Switch_ID].pop();
            id_buffer_vertical[Hyperx_Switch_ID].pop();
            value_buffer_vertical[Hyperx_Switch_ID].pop();
            pull_first_flag_buffer_vertical[Hyperx_Switch_ID].pop();
        }else if(hy2_horizontal_write_output[Hyperx_Switch_ID]){
            Hy_Update_V_ID[Hyperx_Switch_ID] = id_buffer_horizontal[Hyperx_Switch_ID].front();
            Hy_Update_V_DValid[Hyperx_Switch_ID] = 1;

            push_flag_buffer_horizontal[Hyperx_Switch_ID].pop();
            id_buffer_horizontal[Hyperx_Switch_ID].pop();
            value_buffer_horizontal[Hyperx_Switch_ID].pop();
            pull_first_flag_buffer_horizontal[Hyperx_Switch_ID].pop();
        }else if(hy2_input_write_output[Hyperx_Switch_ID]){
            Hy_Update_V_ID[Hyperx_Switch_ID] = id_buffer_input[Hyperx_Switch_ID].front();
            Hy_Update_V_DValid[Hyperx_Switch_ID] = 1;

            push_flag_buffer_input[Hyperx_Switch_ID].pop();
            id_buffer_input[Hyperx_Switch_ID].pop();
            value_buffer_input[Hyperx_Switch_ID].pop();
            pull_first_flag_buffer_input[Hyperx_Switch_ID].pop();
        }else{
            Hy_Update_V_ID[Hyperx_Switch_ID] = 0;
            Hy_Update_V_DValid[Hyperx_Switch_ID] = 0;
        }
    }


    /*vertical_buffer_Receive*/
    if (rst_rd) {
    }else if(tmp_hy2_vertical_v_valid_in_min[Hyperx_Switch_ID]){//这个标记是否会增加依赖链路
        push_flag_buffer_vertical[Hyperx_Switch_ID].push(hy2_vertical_push_flag_in[Hyperx_Switch_ID][tmp_hy2_vertical_v_valid_index_min[Hyperx_Switch_ID]]);
        id_buffer_vertical[Hyperx_Switch_ID].push(hy2_vertical_v_id_in[Hyperx_Switch_ID][tmp_hy2_vertical_v_valid_index_min[Hyperx_Switch_ID]]);
        value_buffer_vertical[Hyperx_Switch_ID].push(hy2_vertical_v_value_in[Hyperx_Switch_ID][tmp_hy2_vertical_v_valid_index_min[Hyperx_Switch_ID]]);
        pull_first_flag_buffer_vertical[Hyperx_Switch_ID].push(hy2_vertical_pull_first_flag_in[Hyperx_Switch_ID][tmp_hy2_vertical_v_valid_index_min[Hyperx_Switch_ID]]);
    }

    /*horizontal_buffer_receive*/
    if (rst_rd) {
    }else if(tmp_hy2_horizontal_v_valid_in_min[Hyperx_Switch_ID]){//这个标记是否会增加依赖链路
        push_flag_buffer_horizontal[Hyperx_Switch_ID].push(hy2_horizontal_push_flag_in[Hyperx_Switch_ID][tmp_hy2_horizontal_v_valid_index_min[Hyperx_Switch_ID]]);
        id_buffer_horizontal[Hyperx_Switch_ID].push(hy2_horizontal_v_id_in[Hyperx_Switch_ID][tmp_hy2_horizontal_v_valid_index_min[Hyperx_Switch_ID]]);
        value_buffer_horizontal[Hyperx_Switch_ID].push(hy2_horizontal_v_value_in[Hyperx_Switch_ID][tmp_hy2_horizontal_v_valid_index_min[Hyperx_Switch_ID]]);
        pull_first_flag_buffer_horizontal[Hyperx_Switch_ID].push(hy2_horizontal_pull_first_flag_in[Hyperx_Switch_ID][tmp_hy2_horizontal_v_valid_index_min[Hyperx_Switch_ID]]);
    }

    /*in->vertical/horizontal_buffer*/
    if (rst_rd) {
        /*buffer initilization*/
    }
    else {
        if(V_Valid_In){
            push_flag_buffer_input[Hyperx_Switch_ID].push(Push_Flag);
            id_buffer_input[Hyperx_Switch_ID].push(V_ID_In);
            value_buffer_input[Hyperx_Switch_ID].push(V_Value_In);
            pull_first_flag_buffer_input[Hyperx_Switch_ID].push(Pull_First_Flag);
            // printf("clk:%d v_id:%d in\n",clk,V_ID_In);
        }
    }

    vertical_buffer_full[Hyperx_Switch_ID] = (id_buffer_vertical[Hyperx_Switch_ID].size() >= FIFO_SIZE);
    horizontal_buffer_full[Hyperx_Switch_ID] = (id_buffer_horizontal[Hyperx_Switch_ID].size() >= FIFO_SIZE);
    input_buffer_full[Hyperx_Switch_ID] = (id_buffer_input[Hyperx_Switch_ID].size() >= FIFO_SIZE);
    for(int i = 0 ; i< VERTICAL_LEN ; i++){
        Vertical_Stage_Full[i] = vertical_buffer_full[Hyperx_Switch_ID];
    }
    for(int i = 0 ; i< HORIZONTAL_LEN ; i++){
        Horizontal_Stage_Full[i] = horizontal_buffer_full[Hyperx_Switch_ID];
    }

    Stage_Full_Hy[Hyperx_Switch_ID] = input_buffer_full[Hyperx_Switch_ID] ;

    *combine_vertical_buffer_size = id_buffer_vertical[Hyperx_Switch_ID].size();
    *combine_id_buffer_vertical_front = id_buffer_vertical[Hyperx_Switch_ID].front();
    *combine_push_flag_buffer_vertical_front = push_flag_buffer_vertical[Hyperx_Switch_ID].front();
    *combine_value_buffer_vertical_front = value_buffer_vertical[Hyperx_Switch_ID].front();
    *combine_pull_first_flag_buffer_vertical_front = pull_first_flag_buffer_vertical[Hyperx_Switch_ID].front();

    *combine_horizontal_buffer_size = id_buffer_horizontal[Hyperx_Switch_ID].size();
    *combine_id_buffer_horizontal_front = id_buffer_horizontal[Hyperx_Switch_ID].front();
    *combine_push_flag_buffer_horizontal_front = push_flag_buffer_horizontal[Hyperx_Switch_ID].front();
    *combine_value_buffer_horizontal_front = value_buffer_horizontal[Hyperx_Switch_ID].front();
    *combine_pull_first_flag_buffer_horizontal_front = pull_first_flag_buffer_horizontal[Hyperx_Switch_ID].front();

    *combine_input_buffer_size = id_buffer_input[Hyperx_Switch_ID].size();
    *combine_id_buffer_input_front = id_buffer_input[Hyperx_Switch_ID].front();
    *combine_push_flag_buffer_input_front = push_flag_buffer_input[Hyperx_Switch_ID].front();
    *combine_value_buffer_input_front = value_buffer_input[Hyperx_Switch_ID].front();
    *combine_pull_first_flag_buffer_input_front = pull_first_flag_buffer_input[Hyperx_Switch_ID].front();

}