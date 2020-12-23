#include "matrix_inverse.h"
#include <math.h>
#include <stdio.h>
#include "matrix_print.h"

int matrix_inverse(double *array, int n, double *inverse, int *vec) {
    double eps = 0.00000001;
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
        vec[n + i] = i;
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
    for (int i = 0; i < n; i++) {
        temp = array[i + n * vec[i]];
        for (int j = 0; j < n; j++) {
            inverse[i + n * j] /= temp;
        }
    }

    return 0;
}
