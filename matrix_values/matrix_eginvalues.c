#include "matrix_eginvalues.h"
#include <math.h>
#include "norm.h"
#include "matrix_print.h"
#include <stdio.h>

/* столбец с номером col  умножить на одну T матрицу с номером T_ind */
void col_on_T_multiply(double *array, int col, int n, double *vec, int T_ind, int *indx){
    double xi = array[indx[0 + 2 * T_ind] + col * n], xj = array[indx[1 + 2 * T_ind] + col * n];
    for(int i = 0; i < n; i++){
        if(i == indx[0 + 2 * T_ind]){
            array[i + col * n] = xi * vec[0 + 2 * T_ind] - xj * vec[1 + 2 * T_ind];
        }else if(i == indx[1 + 2 * T_ind]){
            array[i + col * n] = xi * vec[1 + 2 * T_ind] + xj * vec[0 + 2 * T_ind];
        }
    }
}
/* матрицу умножить на одну матрицу с номером T_ind */
void matrix_on_T_multiply(double *array, int n, double *vec, int T_ind, int *indx){
    for(int col = 0; col < n; col++){
        col_on_T_multiply(array, col, n, vec, T_ind, indx);
    }
}

/* T_1 ... T_m A */
void matrix_on_mT_multiply(double *array, int n, double *vec, int m, int *indx){
    for(int i = 0; i < m; i++){
        matrix_on_T_multiply(array, n, vec, i, indx);
    }
}

/* для стобца ind матрицы вычислить T матрицы */
void compute_T_matrix(double *array, int ind, int n, double *vec, int *indx){
    int T_ind = 0;
    double x = 0, y = 0;
    for(int i = ind + 2; i < n; i++){

        indx[0 + T_ind * 2] = ind + 1;
        indx[1 + T_ind * 2] = i;


        x = array[ind + 1 + ind * n];
        y = array[i + ind * n];

        if(x == 0 && y == 0) {
            vec[0 + T_ind * 2] = 1;
            vec[1 + T_ind * 2] = 0;
        }else{
            vec[0 + T_ind * 2] = x / sqrt(x * x + y * y);
            vec[1 + T_ind * 2] = -y / sqrt(x * x + y * y);
            matrix_on_T_multiply(array, n, vec, T_ind, indx);
        }




        T_ind += 1;
    }

}

void matrix_transpose(double *array, int n){
    double temp = 0;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            temp = array[i + j * n];
            array[i + j * n] = array[j + i * n];
            array[j + i * n] = temp;
        }
    }
}

/* поворачиваем одну колонку */
void one_rotate(double *array, int col, int n, double *vec, int *indx){
    compute_T_matrix(array, col, n, vec, indx);
    matrix_transpose(array, n);
    matrix_on_mT_multiply(array, n, vec, n - 2 - col, indx);
    matrix_transpose(array, n);
}

/* приведение к почти верхнетреугольной методом вращений*/
void rotate_method(double *array, int n, double *vec, int *indx){
    for(int i = 0; i < n - 1; i++){
        one_rotate(array, i, n, vec, indx);

    }
}


/* с. з. матрицы 2 на 2 супер численно нестабильный */
int two_two_eginvalues(double *array, double *vec, int n){
    double d = (array[0] + array[1 + n]) * (array[0] + array[1 + n]) - 4 * (array[0] * array[1 + n] - array[n] * array[1]);
    if (d < 0){
        return 0;
    }else{
        vec[0] = (array[1 + n] + array[0] + sqrt(d))/2;
        vec[1] = (array[1 + n] + array[0] - sqrt(d))/2;
        return 2;
    }
}

/* LR разложение подматрицы n * n*/
void LR_decomposition(double *array, int n, int mat_size){
    for(int i = 1; i < n; i++){
        for(int k = i; k < n; k++){
            array[i + k * mat_size] = array[i + k * mat_size] - array[i + (i - 1) * mat_size] * array[i - 1 + k * mat_size];
        }
    }
    for(int i = 1; i < n; i++){
        array[i + (i - 1) * mat_size] = array[i + (i - 1) * mat_size] / array[i - 1 + (i - 1) * mat_size];
    }
}

/*из LR делаем RL*/
void RL_from_LR(double *array, int n, double *vec, int mat_size){

    for(int i = 0 ; i <  n - 1; i++){
        vec[i] = array[i + 1 + mat_size * (i  + 1)];
    }

    for(int i = 0; i < n; i++){
        for(int k = i; k < n - 1; k++){
            array[i + k * n] = array[i + k * mat_size] + array[i + (k + 1) * mat_size] * array[k + 1 + k * mat_size];
        }
    }
    for(int i = 1; i < n; i++){
        array[i + (i - 1)  * mat_size] = vec[i - 1] * array[i + (i - 1) * mat_size];
    }
}

/* LR со сдвигом*/
void LR_shift(double *array, int n, double s, double *vec, int mat_size, double eps){
    int flag = 1;
    while(flag){
        flag = 0;
        for(int i = 0; i < n - 1; i++) {
            if (fabs(array[i + i * mat_size] - s - array[i + (i - 1) * mat_size] * array[i - 1 + i * mat_size]) < 1e-10) {
                s = 0;
                flag = 1;
            }
        }
    }

    // A - s * E
    for(int i = 0; i < n; i++){
        array[i + i * mat_size] -= s;
    }

    // L, R = A
    // A = R * L
    LR_decomposition(array, n, mat_size);
    RL_from_LR(array, n, vec, mat_size);
    // A + s * E
    for(int i = 0; i < n; i++){
        array[i + i * mat_size] += s;
    }
}

/* итерация LR алгоритма*/
int  one_iter(double *array, int n, double *vec, double eps, double norm, int mat_size){
    double s = array[n - 1 +  (n - 1) * mat_size];
    while(array[n - 1 + (n - 2) * mat_size] > norm  && array[n - 2 + (n - 1) * mat_size] > norm ){
        LR_shift(array, n, s, vec, mat_size, eps);
        s = array[n - 1 +  (n - 1) * mat_size];
    }
    if(!isnan(array[n - 1 + (n - 1) * mat_size])){
        vec[n - 1] = array[n - 1 + (n - 1) * mat_size];
        return 1;
    }
    return 0;
}


/* поиск с.з. матрицы методом LR*/
int LR_method(double *array, int n, double *vec, double eps){
    double norm = 0;
    int count;
    for(int i = n; i > 2; i--){
        norm = inf_norm(array, i, n);
        count += one_iter(array, i, vec, eps, norm, n);
    }
    count += two_two_eginvalues(array, vec, n);
    return count;
}

/* поиск с.з. произовлльной матрицы*/
int matrix_eginvalues(double *array, int n, double eps, double *vec, int *indx){
    int count = 0;
    if(n == 1){
        vec[0] = array[0];
        count += 1;
    }else if(n == 2){
        count += two_two_eginvalues(array, vec, n);
    }else{
        rotate_method(array, n, vec, indx);
	printf("rotate done");
        count += LR_method(array, n, vec, eps);
    }
    return count;
};
