
/*
ECE6740
Final Project - Fall 2020
Function Declarations for stat calculations
*/
#include "softposit.h"
posit32_t Range(posit32_t *);
posit32_t  Median(posit32_t *);
posit32_t *MinMax(posit32_t *); //
posit32_t  Mean(posit32_t *); //
posit32_t  StdDev(posit32_t *,posit32_t);//
posit32_t  IQR(posit32_t *); //
posit32_t Variance(posit32_t *, int); //
posit32_t  Skew(posit32_t *, posit32_t, posit32_t);
void PrintStat(posit32_t*,posit32_t,posit32_t,posit32_t,posit32_t,posit32_t,posit32_t,posit32_t);
