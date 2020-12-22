#include <stdio.h>
#include <stdlib.h>  // for strtol
#include <errno.h>   // for errno
#include <time.h> // for clock
#include "matrix_init.h"
#include "matrix_print.h"
#include "matrix_inverse.h"
#include "norm.h"

int main(int argc, char **argv) {
    int m = 0, n = 0, k = 0;
    char *filename = NULL;
    double* mat = NULL;
    double* inverse = NULL;
    int* vec = NULL;
    double time_taken = 0;
    clock_t t = 0;


    /* read input arguments */
    if (argc == 4) {
        char *p1, *p2, *p3;
        errno = 0;

        n = strtol(argv[1], &p1, 10);
        m = strtol(argv[2], &p2, 10);
        k = strtol(argv[3], &p3, 10);

        if (errno != 0 || *p1 != '\0' || *p2 != '\0' || *p3 != '\0') {
            printf("Invalid argument format \n");
            return -1;
        }
    } else if (argc == 5) {
        char *p1, *p2, *p3;
        errno = 0;

        n = strtol(argv[1], &p1, 10);
        m = strtol(argv[2], &p2, 10);
        k = strtol(argv[3], &p3, 10);
        filename = argv[4];

        if (errno != 0 || *p1 != '\0' || *p2 != '\0' || *p3 != '\0') {
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
    inverse = (double*)malloc(n * n * sizeof(double));
    vec = (int*)malloc(2 * n * sizeof(int));
    if(matrix_init(mat, n, k, filename) != 0){
        printf("Matrix init error. \n");
        free(mat);
        free(inverse);
        free(vec);
        return -1;
    }

    printf("Matrix: \n");
    matrix_print(mat, n, n, m, 1, vec);
    /* compute inverse matrix and time */
    t = clock();
    if(matrix_inverse(mat, n, inverse, vec) != 0){
      free(mat);
      free(inverse);
      free(vec);
      printf("problem with inverse\n");
      return -1;
    }

    t = clock() - t;

    time_taken = ((double)t)/CLOCKS_PER_SEC;

    /* create matrix (again) */
    if(matrix_init(mat, n, k, filename) != 0){
        printf("Matrix init error in second time. \n");
        free(mat);
        free(inverse);
        free(vec);
        return -1;
    }


    /* print results */

    printf("Inverse matrix: \n");
    matrix_print(inverse, n, n, m, 0, vec);
    printf("\n Time taken to find the inverse matrix: %f \n", time_taken);
    printf("\n Residual (2 norm): %10.3e \n", norm(mat, inverse, n, vec));



    /* free memory */
    free(mat);
    free(inverse);
    free(vec);
    return 0;
}
