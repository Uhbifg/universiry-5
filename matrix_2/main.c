#include <stdio.h>
#include <stdlib.h>  // for strtol
#include <errno.h>   // for errno
#include <time.h> // for clock
#include "matrix_init.h"
#include "matrix_print.h"
#include "matrix_inverse_multi.h"
#include "norm.h"


#include <pthread.h>
#include "sys/types.h"





int main(int argc, char **argv) {
    int m = 0, n = 0, k = 0, p = 0;
    char *filename = NULL;
    double* mat = NULL;
    double* inverse = NULL;
    int* vec = NULL;
    double time_taken = 0;
    clock_t t = 0;
    pthread_t	*pids = NULL;	/* массив идентификаторов потоков*/
    struct pthread_arg	*pargs = NULL; /* массив аргументов потоков */

    

    /* read input arguments */
    if (argc == 5) {
        char *p1, *p2, *p3, *p4;
        errno = 0;

        n = strtol(argv[1], &p1, 10);
        m = strtol(argv[2], &p2, 10);
        k = strtol(argv[3], &p3, 10);
        p = strtol(argv[4], &p4, 10);
        if (errno != 0 || *p1 != '\0' || *p2 != '\0' || *p3 != '\0' || *p4 != '\0') {
            printf("Invalid argument format \n");
            return -1;
        }
    } else if (argc == 6) {
        char *p1, *p2, *p3, *p4;
        errno = 0;

        n = strtol(argv[1], &p1, 10);
        m = strtol(argv[2], &p2, 10);
        k = strtol(argv[3], &p3, 10);
        p = strtol(argv[4], &p4, 10);

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

    if(p < 1){
        printf("Invalid thread_num number");
        return -1;
    }
    
    pids = malloc(sizeof(pthread_t) * p);
    pargs = malloc(sizeof(struct pthread_arg) * p);

    /* create matrix */
    mat = (double*)malloc(n * n * sizeof(double));
    inverse = (double*)malloc(n * n * sizeof(double));
    vec = (int*)malloc(n * sizeof(int));
    if(matrix_init(mat, n, k, filename) != 0){
        printf("Matrix init error. \n");
        free(mat);
        free(inverse);
        free(vec);
        free(pids);
        free(pargs);
        return -1;
    }

    printf("Matrix: \n");
    matrix_print(mat, n, n, m);
    /* compute inverse matrix and time */
    t = clock();
    if(matrix_inverse(mat, n, inverse, vec, pids, pargs, p) != 0){
        printf("problem");
        
        free(mat);
        free(inverse);
        free(vec);
        free(pids);
        free(pargs);
        
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
        free(pids);
        free(pargs);
        return -1;
    }


    /* print results */

    printf("Inverse matrix: \n");
    matrix_print(inverse, n, n, m);
    printf("\n Time taken to find the inverse matrix: %f \n", time_taken);
    printf("\n Residual (2 norm): %10.3e \n", norm(mat, inverse, n));



    /* free memory */
    free(mat);
    free(inverse);
    free(vec);
    free(pids);
    free(pargs);

    return 0;
}
