#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calibrate.h"

#ifndef BASE
#define BASE 1500
#endif // BASE

/*Variables*/


/*----------Definitions of static functions----------*/

/*----------Main functions----------*/

double * create_data_bufer(double * obj_buffer, double * tan_buffer,int n, const char polynomial){
    /*object_buffer - massive which keep centers of objects;
      tan_buffer - massive which keep counted tan of test objects;
      n - equation order;
      polynomial - work mode of create SLAE.*/

    double * output = malloc(sizeof(double) * n * (n + 1));
    if(polynomial == 'c'){
        int m = 0, i = 0;
        while(i != n){
            double * temp_bufer = chebyshev_core(obj_buffer[i],3);
            output[m++] = 1.00;
            for(int j = 0;j < n - 1; j++)
                output[m++] = temp_bufer[j];
            output[m++] = tan_buffer[i];
            free(temp_bufer);
            i++;
        }
    }else if(polynomial == 't'){
        int m = 0;
        int i = 0;
        while(i != n){
            output[m++] = 1.00;
            for(int j = 1; j <= n - 1; j++)
                output[m++] = powf(obj_buffer[i],j);
            output[m++] = tan_buffer[i];
            i++;
        }
    }
    return output;
}

double * solve_slae_gauss(int n, double * pointer){
    /*n - equation order;
      pointer - pointer to created buffer which keep tan and coordinate values*/
    double * res = (double*)malloc(sizeof(double)*n);
    double ** matrix = (double**)malloc(sizeof(double*)*n);
    for(int k = 0; k < n; k++)
        matrix[k] = (double*)malloc((n + 1)*sizeof(double));

    for(int i = 0; i < n;i++)
        for(int j = 0; j < (n + 1); j++)
            matrix[i][j] = *pointer++;

    double temp;
    int i = 0;
    while(i != n){
        for(int k = i; k < n; k++){
            temp = matrix[k][i];
            if(temp == 0) continue;
            for(int j = i; j <(n + 1); j++){
                matrix[k][j] = matrix[k][j]/temp;
                if(k >= i + 1)
                    matrix[k][j] = matrix[i][j] - matrix[k][j];
            }
        }
        i++;
    }
    i = n - 1;
    while(i >= 0){
        temp = 0;
        if(i == n-1)
            res[i] = matrix[i][i] == 0? 0 : matrix[i][n]/matrix[i][i];
        else{
            for(int j = i + 1; j < n; j++)
                temp += res[j]*matrix[i][j];
            res[i] = matrix[i][i] == 0? 0 : (matrix[i][n] - temp)/matrix[i][i];
        }
        i--;
    }
    free(matrix);
    return res;
}

double * calibrating_camera(camera_cal * struct_cam){
    double * temp_bufer = create_data_bufer(struct_cam->cal_data, struct_cam->cal_tg,4,'t');
    double * coefs = solve_slae_gauss(4, temp_bufer);
    free(temp_bufer);
    return coefs;
}

/*----------Describing of static functions----------*/
double * chebyshev_core(float value, int n){
    double * output = malloc(sizeof(double) * n);
    output[0] = value;
    output[1] = 2.0*powf(value,2) - 1;
    for(int i = 2; i < n; i++)
        output[i] = 2 * output[0]*output[i - 1] - output[i - 2];
    return output;
}
