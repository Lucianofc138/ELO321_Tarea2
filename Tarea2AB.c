/*
* @file :   "sudokuSinThreads.c"
* @author : Luciano  Flores  Castillo
            Cristian Herrera Leizgold
* @date :   8/11/2020
* @brief :  Código para la tarea 2 de ELO 321, semestre 2020-2
*/

#include <stdbool.h> 
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>

#include "sudokuUtils.h"

int sudoku_array[9][9] = {  {1,2,3,4,5,6,7,8,9},
                            {7,8,9,1,2,3,4,5,6},
                            {4,5,6,7,8,9,1,2,3},
                            {2,3,4,5,6,7,8,9,1},
                            {8,9,1,2,3,4,5,6,7},
                            {5,6,7,8,9,1,2,3,4},
                            {3,4,5,6,7,8,9,1,2},
                            {9,1,2,3,4,5,6,7,8},
                            {6,7,8,9,1,2,3,4,5} };

bool rows_checked[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
bool cols_checked[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
bool sub_grids_checked[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 


bool validity_check(struct board_zone zone2check);
bool is_valid_row(int row);
bool is_valid_col(int col);
bool is_valid_box(zone zone2check);
void routineA(long int iterations);
void routineB(long int iterations);

int main(int argc, char* argv[]){
    int iterations;
    if (argc > 1 )
        iterations = atoi(argv[1]);
    else
        iterations = 10000;
    printf("Ciclo sin threads: \n");
    routineA(iterations);
    printf("\n Ciclo con threads en OpenMP: \n");
    routineB(iterations);
    return 0;
    
}

bool validity_check(zone zone2check){
    bool check_row = (zone2check.init_row==zone2check.fin_row 
                      &&  zone2check.init_col == 0
                      &&  zone2check.fin_col  == 8 );
    bool check_col = (zone2check.init_col==zone2check.fin_col 
                      &&  zone2check.init_row == 0
                      &&  zone2check.fin_row  == 8 );
    bool check_box = (  (zone2check.fin_col - zone2check.init_col)==2 && 
                        (zone2check.fin_row - zone2check.init_row)==2 && 
                        (zone2check.init_row % 3) == 0 &&
                        (zone2check.init_col % 3) == 0);
    if (check_row)
        return is_valid_row(zone2check.init_row);
    else if(check_col)
        return is_valid_col(zone2check.init_col);
    else if(check_box)
        return is_valid_box(zone2check);
    else
        return false;
    
}

bool is_valid_row(int row){
    bool numbers_found[9] = {0,0,0,0,0,0,0,0,0};
    int i;
    for(i=0; i<9; i++){
        int num = sudoku_array[row][i];
        if(num != 0){
            if ( numbers_found[num-1]==1 )
                return false;
            else
                numbers_found[num-1] = 1; 
        }
    }
    return true;
}
bool is_valid_col(int col){
    bool numbers_found[9] = {0,0,0,0,0,0,0,0,0};
    int i;
    for(i=0; i<9; i++){
        int num = sudoku_array[i][col];
        if(num != 0){
            if ( numbers_found[num-1]==1 )
                return false;
            else
                numbers_found[num-1] = 1; 
        }
    }
    return true;
}

bool is_valid_box(zone zone2check){
    bool numbers_found[9] = {0,0,0,0,0,0,0,0,0};
    int row, col;
    for(row = zone2check.init_row;  row < zone2check.fin_row+1;  row++){
        for(col = zone2check.init_col;  col < zone2check.fin_col+1; col++){
            int num = sudoku_array[row][col];
            if(num != 0){
                if ( numbers_found[num-1]==1 )
                    return false;
                else
                    numbers_found[num-1] = 1; 
            }
        }
    }
    return true;
}
void restart_check_arrays(){
    int i;
    for (i = 0; i < 9; i++)
    {
        rows_checked[i] = 0;
        cols_checked[i] = 0;
        sub_grids_checked[i] = 0;
    }
}

bool get_result(){
    bool result = true;
    int i;
    for(i=0; i<9; i++){
        result = result && rows_checked[i];
        result = result && cols_checked[i];
        result = result && sub_grids_checked[i];
    }
    return result;
}

void routineA(long int iterations){
    struct timeval tv1, tv2;
    double sum = 0;
    
    int j;
    for(j=0; j<iterations; j++)
    {
        gettimeofday(&tv1, NULL);
        bool result = true;
        int i;
        for (i = 0; i < 9; i++)
        {
            zone row = get_row_by_pos(i);
            rows_checked[i] = validity_check(row);

            zone col = get_col_by_pos(i);
            cols_checked[i] = validity_check(col);
            
            zone box = get_box_by_pos(i);
            sub_grids_checked[i] = validity_check(box);
        }
        gettimeofday(&tv2, NULL);
        double time = ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000.0 +
                       (double) (tv2.tv_sec - tv1.tv_sec));
        sum += time;
        restart_check_arrays();
    }
    
    double mean_time_sec = sum /((double)iterations);
    printf ("Time = %f [us]\n", mean_time_sec*1000000);
}

void routineB(long int iterations){
    struct timeval tv1, tv2;
    double sum = 0;
    
    int j;
    for(j=0; j<iterations; j++){
        gettimeofday(&tv1, NULL);
        #pragma omp parallel
        {
            int i;
            #pragma omp for
            for (i = 0; i < 9; i++)
            {
                zone row = get_row_by_pos(i);
                rows_checked[i] = validity_check(row);

                zone col = get_col_by_pos(i);
                cols_checked[i] = validity_check(col);
                
                zone box = get_box_by_pos(i);
                sub_grids_checked[i] = validity_check(box);

                if(get_result) gettimeofday(&tv2, NULL);
            }
        }
        double time = ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000.0 +
                               (double) (tv2.tv_sec - tv1.tv_sec));
        sum += time;
        restart_check_arrays();
    }
    
    double mean_time_sec = sum /((double)iterations);
    printf ("Time = %f [us]\n", mean_time_sec*1000000);
}