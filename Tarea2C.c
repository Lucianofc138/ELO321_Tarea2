#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#define NUM_THREADS 9

struct row_col {
	int init_row;
	int fin_row;
	int init_col;
	int fin_col;
};
struct timeval tv1, tv2;

int sudoku_array[9][9] = {{1,2,3,4,5,6,7,8,9},
                          {7,8,9,1,2,3,4,5,6},
						  {4,5,6,7,8,9,1,2,3},
						  {2,3,4,5,6,7,8,9,1},
						  {8,9,1,2,3,4,5,6,7},
						  {5,6,7,8,9,1,2,3,4},
						  {3,4,5,6,7,8,9,1,2},
						  {9,1,2,3,4,5,6,7,8},
						  {6,7,8,9,1,2,3,4,5} };


/*
int sudoku_array[9][9] = {{1,2,3,4,5,6,7,8,9},
                          {7,8,9,1,2,3,4,5,6},
						  {4,5,6,7,8,9,1,2,3},
						  {3,3,4,5,6,7,8,9,9},
						  {8,9,1,2,3,4,5,6,7},
						  {5,6,7,8,9,1,2,3,4},
						  {3,4,5,6,7,8,9,1,2},
						  {9,1,2,3,4,5,6,7,8},
						  {7,7,8,9,1,1,3,4,4} };
*/						  


int rows_checked[9] = {0,0,0,0,0,0,0,0,0}; /* this data is shared by the thread(s) */
int cols_checked[9] = {0,0,0,0,0,0,0,0,0};
int sub_grid_checked[9] = {0,0,0,0,0,0,0,0,0};
void *validity_check_square(void *param); /* threads call this function*/
void *validity_check_row(void *param); /* threads call this function*/
void *validity_check_column(void *param); /* threads call this function*/
int get_zona(struct row_col * zone); 


int main(int argc, char *argv[])
{
	bool r = true;
	bool c = true;
	bool g = true;

	struct row_col fil_col;
	struct row_col zona[9];
	fil_col.init_row = 0;
	fil_col.fin_row = 8;
	fil_col.init_col = 0;
	fil_col.fin_col = 8;

	zona[0].init_row = 0;
	zona[0].fin_row = 2;
	zona[0].init_col = 0;
	zona[0].fin_col = 2;

	zona[1].init_row = 0;
	zona[1].fin_row = 2;
	zona[1].init_col = 3;
	zona[1].fin_col = 5;

	zona[2].init_row = 0;
	zona[2].fin_row = 2;
	zona[2].init_col = 6;
	zona[2].fin_col = 8;

	zona[3].init_row = 3;
	zona[3].fin_row = 5;
	zona[3].init_col = 0;
	zona[3].fin_col = 2;

	zona[4].init_row = 3;
	zona[4].fin_row = 5;
	zona[4].init_col = 3;
	zona[4].fin_col = 5;

	zona[5].init_row = 3;
	zona[5].fin_row = 5;
	zona[5].init_col = 6;
	zona[5].fin_col = 8;

	zona[6].init_row = 6;
	zona[6].fin_row = 8;
	zona[6].init_col = 0;
	zona[6].fin_col = 2;

	zona[7].init_row = 6;
	zona[7].fin_row = 8;
	zona[7].init_col = 3;
	zona[7].fin_col = 5;

	zona[8].init_row = 6;
	zona[8].fin_row = 8;
	zona[8].init_col = 6;
	zona[8].fin_col = 8;

	//pthread_t tid; /* the thread identifier */
	pthread_t tid_rows; 
	pthread_t tid_cols; 
	pthread_t tid_square[9]; 
	pthread_attr_t attr_rows; /* set of thread attributes */
	pthread_attr_t attr_cols; /* set of thread attributes */
	pthread_attr_t attr_square[9]; /* set of thread attributes */
	

	/* get the default attributes */
	pthread_attr_init(&attr_rows);
	pthread_attr_init(&attr_cols);
    int i = 0;
    int l = 0;
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_attr_init(&attr_square[i]);
	}

	/* create the thread */
    gettimeofday(&tv1, NULL);
	pthread_create(&tid_rows,&attr_rows,validity_check_row,&fil_col);
	pthread_create(&tid_cols,&attr_cols,validity_check_column,&fil_col);
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid_cols,&attr_cols,validity_check_square,&zona[i]);
	}

	/* wait for the thread to exit */
	pthread_join(tid_rows,NULL);
	pthread_join(tid_cols,NULL);
    gettimeofday(&tv2, NULL);

    printf("filas:");
	for (l = 0; l < 9; l++) {
	    printf(" %d ",rows_checked[l]); 
	    if(rows_checked[l] == 0) {
		    r = false;
		    //break;
	    }
    }
    printf("\n");
    printf("columnas:");
	for (l = 0; l < 9; l++) {
	    printf(" %d ",cols_checked[l]); 
	    if(cols_checked[l] == 0) {
		    c = false;
		    //break;
	    }	  
    }
    printf("\n");
    printf("zonas:");
	for (l = 0; l < 9; l++) {
	    printf(" %d ",sub_grid_checked[l]);
	    if(sub_grid_checked[l] == 0) {
		    g = false;
		    //break;
	    }	   
    }
    printf("\n");

	if (r&c&g) {
		printf("Es una solucion de sudoku factible\n");
	}
	else {
		printf("No es una solucion de sudoku factible\n");
	}
    printf("La comprobacion de la solucion del sudoku mediante threads demora %f segundos\n",(double) (tv2.tv_usec - tv1.tv_usec)/1000000.0 + (double) (tv2.tv_sec - tv1.tv_sec));
	//printf("sum = %d\n",cols_checked[0]);
}
/* The thread will begin control in this function */
void *validity_check_row(void *paramm)
{
	struct row_col *param = (struct row_col*)paramm; 
	//printf("sum = %d\n",mqw->fin_row);
	//printf("sum = %d\n",0);
  //int aux[9]; 
  int j = 0;
  int i = 0;
  int m = 0;
  for (j = param->init_row; j <= param->fin_row; j++) { //chequea filas
	  	int aux[9] = {0,0,0,0,0,0,0,0,0}; //se inicia en 0 cada posicion del arreglo de comprobacion
		int err_flag = 0; //bandera de error
	    for (i = param->init_col; i <= param->fin_col; i++) { //chequea columnas
			for (m = 0; m < 9; m++) {
				if (aux[m] == 0) { //se agrega el numero respectivo de fila y columna del sudoku_array al arreglo de comprobacion aux
					aux[m] = sudoku_array[j][i];
					//printf("aux %d\n",aux[m]); 
					break;
				}
				else {
					if (aux[m] == sudoku_array[j][i]) { //se repite un numero en la fila respectiva
						err_flag = 1;
						break;
					}
				}
				//printf("aux %d\n",aux[m]); 
								
			}
			if (err_flag == 1) {
				break; //se termina el chequeo de fila pues hay un error
			}
  		}
		if (err_flag == 0) {
			rows_checked[j] = 1; //no hubo error en la fila
			//printf("hola");
		} 
  }
  pthread_exit(0);
}


void *validity_check_column(void *paramm)
{
	struct row_col *param = (struct row_col*)paramm; 
	//printf("sum = %d\n",mqw->fin_row);
	//printf("sum = %d\n",0);
  //int aux[9]; 
  int j = 0;
  int i = 0;
  int m = 0;
  for (j = param->init_col; j <= param->fin_col; j++) { //chequea filas
	  	int aux[9] = {0,0,0,0,0,0,0,0,0}; //se inicia en 0 cada posicion del arreglo de comprobacion
		int err_flag = 0; //bandera de error
	    for (i = param->init_row; i <= param->fin_row; i++) { //chequea columnas
			for (m = 0; m < 9; m++) {
				if (aux[m] == 0) { //se agrega el numero respectivo de fila y columna del sudoku_array al arreglo de comprobacion aux
					aux[m] = sudoku_array[i][j];
					//printf("aux %d\n",aux[m]); 
					break;
				}
				else {
					if (aux[m] == sudoku_array[i][j]) { //se repite un numero en la fila respectiva
						err_flag = 1;
						break;
					}
				}
				//printf("aux %d\n",aux[m]); 
								
			}
			if (err_flag == 1) {
				break; //se termina el chequeo de fila pues hay un error
			}
  		}
		if (err_flag == 0) {
			cols_checked[j] = 1; //no hubo error en la fila
			//printf("hola");
		} 
  }
  pthread_exit(0);
}


void *validity_check_square(void *paramm)
{
  struct row_col *param = (struct row_col*)paramm; 
	//printf("sum = %d\n",mqw->fin_row);
	//printf("sum = %d\n",0);
  //int aux[9]; 
  int zona = 0;
  int m = 0;
  int aux[9] = {0,0,0,0,0,0,0,0,0}; //se inicia en 0 cada posicion del arreglo de comprobacion
  int err_flag = 0; //bandera de error
  int j = 0;
  int i = 0;
  zona = get_zona(param);
  for (j = param->init_row; j <= param->fin_row; j++) { //chequea filas
	    for (i = param->init_col; i <= param->fin_col; i++) { //chequea columnas
			for (m = 0; m < 9; m++) {
				if (aux[m] == 0) { //se agrega el numero respectivo de fila y columna del sudoku_array al arreglo de comprobacion aux
					aux[m] = sudoku_array[j][i];
					//printf("aux %d\n",aux[m]); 
					break;
				}
				else {
					if (aux[m] == sudoku_array[j][i]) { //se repite un numero en la fila respectiva
						err_flag = 1;
						break;
					}
				}
		    }
			if (err_flag == 1) {
				break; //se termina el chequeo de fila pues hay un error
			}	
  		}
		if (err_flag == 1) {
			break;
			//printf("hola");
		} 
  }
  if (err_flag == 0) {
	  sub_grid_checked[zona] = 1;
  }
  pthread_exit(0);
}

int get_zona(struct row_col * zone) {
	int zona = 10;
	switch(zone->init_row) 
	{
		case 0:
			switch (zone->init_col)
			{
				case 0:
					zona = 0;
				break;
				case 3:
					zona = 1;
				break;
				case 6:
					zona = 2;
				break;
			}
		break;
		case 3:
			switch (zone->init_col)
			{
				case 0:
					zona = 3;
				break;
				case 3:
					zona = 4;
				break;
				case 6:
					zona = 5;
				break;
			}
		break;
		case 6:
			switch (zone->init_col)
			{
				case 0:
					zona = 6;
				break;
				case 3:
					zona = 7;
				break;
				case 6:
					zona = 8;
				break;
			}
		break;	
	}
	return zona;
}