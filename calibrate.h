#ifndef CAL_H
#define CAL_H


/*Variables*/
typedef struct{
    double cal_data[4];
    double cal_tg[4];
} camera_cal;

/*DEFINITIONS*/
double * create_data_bufer(double * obj_buffer, double * tan_buffer,int n, const char polynomial);
double * solve_slae_gauss(int n, double * pointer);
double * calibrating_camera(camera_cal * struct_cam);
double * chebyshev_core(float value, int n);
#endif // CAL_H
