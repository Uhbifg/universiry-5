#ifndef ZACHET1_MATRIX_INVERSE_H
#define ZACHET1_MATRIX_INVERSE_H


#include <pthread.h>
#include "sys/types.h"

struct pthread_arg {
    int	p_current, p_total, n;
    double	*array;
    double *inverse;
    int indx;
    double temp;
    int *vector;
};	/* объявление структуры для аргументов потока */



int  matrix_inverse(double *array, int n, double *inverse, int *vec, pthread_t *pids, struct pthread_arg *pargs, int p);
#endif //ZACHET1_MATRIX_INVERSE_H
