
/*
ECE6740
Final Project - Fall 2020
Function Declarations for stat calculations
*/
#include "softposit.h"
float  Range(float *);
float  Median(float *);
float *MinMax(float *); //
float  Mean(float *); //
float  StdDev(float *,float);//
float  IQR(float *); //
float  Variance(float *, int); //
float  Skew(float *, float, float);
void   PrintStat(float*,float,float,float,float,float,float,float);
float  RMSE_Calc(float *, float *);
float MAE_Calc(float *, float *);
float L2_Norm_Calc(float*, float *);
float L2_Inf_Calc(float*);
