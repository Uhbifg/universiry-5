#include "matrix_init.h"
#include <stdio.h>
#include <stdlib.h>

double f(int k, int n, int i, int j) {
    switch (k) {
        case 1:
            if (i > j) {
                return n - i;
            } else {
                return n - j;
            }
        case 2:
            if(i == j){
                return 2;
            }
            if (i - j == 1 || j - i == 0) {
                return -1;
            } else {
                return 0;
            }
        case 3:
            if (i == j && i < n - 1) {
                return 1;
            } else if(j == n - 1){
                return i + 1;
            }else if(i == n - 1){
                return j + 1;
            }else{
                return 0;
            }
        case 4:
            return 1 / (i + j + 1.0);
    }
    return 0;
}

int matrix_init(double *array, int n, int k, char *filename) {
    FILE *inp;
    if (k == 0) {
        inp = fopen(filename, "r");
        if (inp == NULL) {
            printf("Cannot open file.\n");
            return -1;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (fscanf(inp, "%lf,", &array[i + n * j]) != 1) {
                    printf("File read error. \n");
                    fclose(inp);
                    return -1;
                }
                if(j != n - 1){
                    if(fgetc(inp) != ' '){
                        printf("Invalid matrix format. \n");
                        fclose(inp);
                        return -1;
                    }
                }
            }
            if(fgetc(inp) != '\n'){
                printf("Invalid matrix format. \n");
                fclose(inp);
                return -1;
            }
        }
        fclose(inp);
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                array[i + n * j] = f(k, n, i, j);
            }
        }
    }
    return 0;
}
