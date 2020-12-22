#include "matrix_inverse.h"
#include <math.h>
#include <stdio.h>
/* swap i, j columns of array */
void swap_columns(double *array, int i, int j, int n) {
    double temp = 0;

    for (int k = 0; k < n; k++) {
        temp = array[k + j * n];
        array[k + j * n] = array[k + i * n];
        array[k + i * n] = temp;
    }
}


int matrix_inverse(double *array, int n, double *inverse, int *vec) {
    double eps = 0.000001;
    double temp = 0;
    int temp_col = 0;
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

    int a = 0;
    /* gauss elimination with pivoting by row */
    for (int i = 0; i < n; i++) {
        temp = array[i + i * n];
        temp_col = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(array[i + j * n]) > temp) {
                temp_col = j;
                temp = fabs(array[i + j * n]);
                a = 1;
            }
        }
        if(a == 1){
            a = 0;
            int b = vec[i];
            vec[i] = vec[temp_col];
            vec[temp_col] = b;
            swap_columns(array, i, temp_col, n);
            swap_columns(inverse, i, temp_col, n);
        }
        if(fabs(array[i + i * n]) < eps){
          printf("(whooops, smthing wrong)\n");
          return -1;
        }
        for (int j = 0; j < n; j++) {

            if (j != i) {

                temp = array[j + n * i] / array[i + n * i];
                for (int k = 0; k < n; k++) {

                    array[j + n * k] -= array[i + n * k] * temp;
                    inverse[j + n * k] -= inverse[i + n * k] * temp;
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {

        temp = array[i + n * i];
        for (int j = 0; j < n; j++) {

            inverse[i + n * j] = inverse[i + n * j] / temp;
        }
    }

    /* меняем все обратно */
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            array[vec[i] + n * j] = inverse[i + n * j];
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) {
            inverse[i + n * j] = array[i + n * j];
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            array[j + n * vec[i]] = inverse[j + n * i];
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) {
            inverse[i + n * j] = array[i + n * j];
        }
    }
    return 0;
}
