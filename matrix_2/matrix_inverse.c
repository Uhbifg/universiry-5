#include "matrix_inverse.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include "sys/types.h"
#include "synchronize.h"


void *thread_program(void *arg)
{
	int	*res; /* возвращаемое функцией (потоком) значение */
	struct pthread_arg	*parg = arg;

}

int  matrix_inverse(double *array, int n, double *inverse, int *vec, pthread_t *pids, struct pthread_arg *pargs, int p) {
    double eps = 0.00000001;
    double temp = 0;
    int temp_col = 0;
	  void *pres;
    int	res = 0, tmp;
    /* Initialization inverse matrix by the identity matrix*/
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                inverse[i + j * n] = 1.0;
            } else {
                inverse[i + j * n] = 0;
            }
        }
        vec[i] = i;
    }


    for (int i = 0; i < p; i++) { /* для каждого потока задаем начальные аргументы*/
        pargs[i].p_current = i; /* номер потока*/
        pargs[i].p_total = p; /* общее количество потоков*/
        pargs[i].n = n; /* размерность матицы */
        pargs[i].array = array; /* матрица */
        pargs[i].inverse = inverse;
        pargs[i].indx = 0;
        pargs[i].vector = vec;
    }

    int a = 0, ba = 0;
    /* gauss elimination with pivoting by row */
    for (int i = 0; i < n; i++) {
        temp = array[i + vec[i] * n];
        temp_col = vec[i];
        for (int j = 0; j < n; j++) {
            if (fabs(array[i + vec[j] * n]) > temp) {
                temp_col = vec[j];
                temp = fabs(array[i + vec[j] * n]);
                a = 1;
                ba = j;
            }
        }
        if(a == 1){
            a = 0;
            int b = vec[i];
            vec[i] = temp_col;
            vec[ba] = b;
            vec[n + temp_col] = i;
            vec[n + b] = ba;
        }
        if(fabs(array[i + vec[i] * n]) < eps){
          printf("(whooops, smthing wrong)\n");
          return -1;
        }
        for (int j = 0; j < n; j++) {
            if (j != i) {

                temp = array[j + n * vec[i]] / array[i + n * vec[i]];
                for (int k = 0; k < n; k++) {
                    array[j + n * k] -= array[i + n * k] * temp;
                    inverse[j + n * k] -= inverse[i + n * k] * temp;
                }
            }
        }
    }


    for (int j = 0; j < p; j++){
            tmp = pthread_join(pids[j], &pres);

            if (tmp != 0) {
                fprintf(stderr, "Error: %s:%d", __FILE__, __LINE__);
                return -2;
            }

            if (*((int *)pres) != 0)
                res = 1;

            free(pres);
        }
    }


    for (int i = 0; i < n; i++) {
        temp = array[i + n * vec[i]];
        for (int j = 0; j < n; j++) {
            inverse[i + n * j] /= temp;
        }
    }
    return 0;
}
