#include "matrix_inverse.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix_print.h"
#include <pthread.h>
#include "sys/types.h"
#include "synchronize.h"



void *thread_program(void *arg)
{
	int	*res; /* возвращаемое функцией (потоком) значение */
	struct pthread_arg	*parg = arg;
  double eps = 0.00000001;
  double temp = 0;
  int temp_col = 0;
  int a = 0, ba = 0;




   /* gauss elimination with pivoting by row */
   for (int i = 0; i < parg->n; i++) {
      synchronize(parg->p_total);
        if(parg->p_current == 0){
          temp = parg->array[i + parg->vec[i] * parg->n];
          temp_col = parg->vec[i];
          for (int j = 0; j < parg->n; j++) {
              if (fabs(parg->array[i + parg->vec[j] * parg->n]) > temp) {
                  temp_col = parg->vec[j];
                  temp = fabs(parg->array[i + parg->vec[j] * parg->n]);
                  a = 1;
                  ba = j;
              }
          }
          if(a == 1){
              a = 0;
              int b = parg->vec[i];
              parg->vec[i] = temp_col;
              parg->vec[ba] = b;
              parg->vec[parg->n + temp_col] = i;
              parg->vec[parg->n + b] = ba;
          }
        }
        synchronize(parg->p_total);
       if(fabs(parg->array[i + parg->vec[i] * parg->n]) < eps){
         printf("(whooops, smthing wrong)\n");
         *res = -1;
         break;
       }else{
         for (int j =  parg->p_current; j < parg->n; j +=  parg->p_total) {
             if (j != i) {
                 temp = parg->array[j + parg->n * parg->vec[i]] / parg->array[i + parg->n * parg->vec[i]];
                 for (int k = 0; k < parg->n; k++) {
                     parg->array[j + parg->n * k] -= parg->array[i + parg->n * k] * temp;
                     parg->inverse[j + parg->n * k] -= parg->inverse[i + parg->n * k] * temp;
                 }
             }
         }
       }
   }

   synchronize(parg->p_total);

   for (int i = parg->p_current; i < parg->n; i += parg->p_total) {
       for (int j = 0; j < parg->n; j++) {
           parg->inverse[i + parg->n * j] /= parg->array[i + parg->n * parg->vec[i]];
       }
   }

  }


int  matrix_inverse(double *array, int n, double *inverse, int *vec, pthread_t *pids, struct pthread_arg *pargs, int p) {
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
        vec[n + i] = i;
    }


    for (int i = 0; i < p; i++) { /* для каждого потока задаем начальные аргументы*/
        pargs[i].p_current = i; /* номер потока*/
        pargs[i].p_total = p; /* общее количество потоков*/
        pargs[i].n = n; /* размерность матицы */
        pargs[i].array = array; /* матрица */
        pargs[i].inverse = inverse;
        pargs[i].vec = vec;
        /* создаем поток, исполняющий thread_program с заданными аргументами */
		    res = pthread_create(pids + i, NULL, thread_program, pargs + i);
		    /* запись 'pids + i' равнозначна записи '&pids[i]' */

    		if (res != 0) {
            printf("err1\n");
    			  return -1;
    		}
    }


    for(int i = 0; i < p; i++){
      pthread_join(pids[i], &pres);
    }




    return 0;
}
