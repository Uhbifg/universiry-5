#include "norm.h"
#include <math.h>


double norm(double *array, double *inverse, int n, int *vec){
    double ans = 0;
    double element = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            element = 0;
            for(int k = 0; k < n; k++){
                element += array[i + k * n] * inverse[vec[n + k] + n * j];
            }
            if(i == j){
                element -= 1;
            }
            ans += element * element;
        }
    }
    return sqrt(ans);
}
