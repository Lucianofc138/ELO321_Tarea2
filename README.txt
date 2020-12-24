Integrantes: 
-Luciano Flores Castillo 201621023-7 luciano.flores@sansano.usm.cl
-Cristian Herrera Leizgold 201621049-0 cristian.herreral@sansano.usm.cl


La tarea se hizo en 2 archivos distintos donde Tarea2AB.c es el código para
la parte A y B de la tarea mientras que Tarea2C.c es el código para la parte
C. En particular la primera parte chequea el sudoku y evalua el rendimiento 
de los codigos sin threads y B con threads de OpenMP. Mientras que la parte C 
chequea el sudoku y evalua el rendimiento deL codigo con threads de POSIX API.

Con el terminal en la carpeta del proyecto...

    Para compilar:
        gcc Tarea2AB.c sudokuUtils.h -o tarea2AB_exec -fopenmp 
        gcc Tarea2C.c  -o tarea2C_exec -pthread 

    Para correr: ( Remplazar <numero_iteraciones> por un numero natural )
        ./tarea2AB_exec <numero_iteraciones>
        ./tarea2C_exec <numero_iteraciones>