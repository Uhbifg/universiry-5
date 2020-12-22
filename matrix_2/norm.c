#include "norm.h"
#include <math.h>

double norm1(double *array, double *vec, int n){
    double ans1 = 0;
    for(int i = 0; i < n; i++){
        ans1 += array[i + n * i] - vec[i];
    }
    return ans1;
}

double norm2(double *array, double *vec, int n){
    double ans2 = 0, temp = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            ans2 += array[i + n * j] * array[i + n * j];
        }
        temp += vec[i] * vec[i];
    }
    ans2 = fabs(sqrt(ans2) - temp);
    return ans2;
}


double inf_norm(double *array, int n, int matrix_size){
    double norm = 0, temp = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            temp += fabs(array[i + matrix_size * j]);
        }
        if(temp > norm){
            norm = temp;
        }
        temp = 0;
    }
    return norm;
}