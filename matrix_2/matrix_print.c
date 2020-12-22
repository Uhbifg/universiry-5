#include "matrix_print.h"
#include <stdio.h>


void matrix_print(double *array, int n, int l, int m, int flag, int *vec){
    for(int i = 0; i < n; i++){
        if(i == m){
            break;
        }
        for(int j = 0; j < l; j++){
            if(j == m){
                break;
            }
            if(j != l - 1){
              if(flag == 1){
                printf("%10.3e ", array[i + j * n]);
              }else{
                printf("%10.3e ", array[vec[n + i] + n * j]);
              }
            }else{
              if(flag == 1){
                printf("%10.3e",array[i + j * n]);
              }else{
                printf("%10.3e", array[vec[n + i] + n * j]);
              }
            }
        }
        printf("\n");
    }
    printf("\n");
}
