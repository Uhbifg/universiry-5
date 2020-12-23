#include <stdio.h>
#include <stdlib.h>  // for strtol
#include <errno.h>   // for errno
#include <time.h> // for clock
#include "matrix_init.h"
#include "matrix_print.h"
#include "matrix_eginvalues.h"
#include "norm.h"

int main(int argc, char **argv) {
    int m = 0, n = 0, k = 0;
    double eps;
    char *filename = NULL;
    double* mat = NULL;
    double time_taken = 0;
    double *vec = NULL;
    int *indx = NULL;
    int count = 0;
    clock_t t = 0;


    /* read input arguments */
    if (argc == 5) {
        char *p1, *p2, *p3, *p4;
        errno = 0;

        n = strtol(argv[1], &p1, 10);
        m = strtol(argv[2], &p2, 10);
        eps = strtod(argv[3], &p3);
        k = strtol(argv[4], &p4, 10);


        if (errno != 0 || *p1 != '\0' || *p2 != '\0' || *p3 != '\0' || *p4 != '\0') {
            printf("Invalid argument format \n");
            return -1;
        }
    } else if (argc == 6) {
        char *p1, *p2, *p3, *p4;
        errno = 0;

        n = strtol(argv[1], &p1, 10);
        m = strtol(argv[2], &p2, 10);
        eps = strtod(argv[3], &p3);
        k = strtol(argv[4], &p4, 10);

        filename = argv[5];

        if (errno != 0 || *p1 != '\0' || *p2 != '\0' || *p3 != '\0' || *p4 != '\0') {
            printf("Invalid argument format \n");
            return -1;
        }
    } else {
        printf("Invalid argument format \n");
        return -1;
    }


    if(k < 0 || k > 4){
        printf("Invalid argument format \n");
        return -1;
    }

    /* create matrix */
    mat = (double*)malloc(n * n * sizeof(double));
    vec = (double*)malloc(4 * n * sizeof(double));
    indx = (int*)malloc(4 * n * sizeof(int));
    if(matrix_init(mat, n, k, filename) != 0){
        printf("Matrix init error. \n");
        free(mat);
        free(vec);
        free(indx);
        return -1;
    }

    printf("Matrix: \n");
    matrix_print(mat, n, n, m);

    /* compute eginvalues  and time */
    t = clock();
    count = matrix_eginvalues(mat, n, eps, vec, indx);
    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;

    /* create matrix (again) */
    if(matrix_init(mat, n, k, filename) != 0){
        printf("Matrix init error in second time. \n");
        free(mat);
        free(vec);
        free(indx);
        return -1;
    }


    /* print results */

    printf("Eginvalues: \n");
    matrix_print(vec, count, 1, m);
    printf("\n Time taken to find the eginvalues: %f \n", time_taken);
    printf("\n Norms: \n");
    printf("1) %10.3e \n", norm1(mat, vec, n));
    printf("2) %10.3e \n", norm2(mat, vec, n));




    /* free memory */
    free(mat);
    free(vec);
    free(indx);
    return 0;
}
