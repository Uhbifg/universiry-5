#include "matrix_print.h"
#include <stdio.h>
#include <math.h>

void matrix_print(double *array, int n, int l, int m){
    for(int i = 0; i < n; i++){
        if(i == m){
            break;
        }
        for(int j = 0; j < l; j++){
            if(j == m){
                break;
            }
            printf("%10.3e", array[i + j * n]);
          }
          printf("\n");
        }
    printf("\n");
}

void matrix_print_int(int *array, int n, int l, int m){
    for(int i = 0; i < n; i++){
        if(i == m){
            break;
        }
        for(int j = 0; j < l; j++){
            if(j == m){
                break;
            }
            if(j != l - 1){
              printf("%i ", array[i + j * n]);
            }else{
              printf("%i", array[i + j * n]);
            }

        }
        printf("\n");
    }
    printf("\n");
}
