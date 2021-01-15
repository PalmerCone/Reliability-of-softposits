#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "Read.h"
#include <string.h>


#define Data_Size 2869440

float L2_Norm_Calc(float* number, float* flip){
  int i;
  float sum=0.0;
  float L2_Norm;

    for(i=0;i<Data_Size;i++){
        sum+=pow(number[i]-flip[i],2);
      }
  L2_Norm=sqrtf(sum);
  return(L2_Norm);
}

float L2_Inf_Calc(float* number){
  int i;
  float L2_Inf = 0.0;

    for(i=0;i<Data_Size;i++){
        if (fabs(number[i]) > L2_Inf) L2_Inf = fabs(number[i]);
      }
  return(L2_Inf);
}

float MAE_Calc(float *number, float *flip){
  int i;
  float sum=0.0;
  float MAE;
  for(i=0;i<Data_Size;i++){
    sum+=fabs(number[i]-flip[i]);
  }
  MAE=sum/Data_Size;
  return(MAE);
}

float RMSE_Calc(float *number, float *flip){
  int i;
  float sum=0.0;
  float RMSE;


    for(i=0;i<Data_Size;i++){

        sum+=pow(flip[i]-number[i],2);

      }
  RMSE=sqrt(sum/Data_Size);
  return(RMSE);

}

float* MinMax(float *number){
  int i;
  float min=FLT_MAX;
  float max=FLT_MIN;
  static float MinMax[2];
  int total;

   for(i = 0; i < Data_Size; i++){
     //sum
     total = total + number[i];

     //min
     if(number[i] < min){
       min = number[i];
     }

     //max
     if(number[i] > max){
       max = number[i];
     }

   }
   MinMax[0]=min;
   MinMax[1]=max;
   return(MinMax);
}

float Mean(float *number){
  float total=0;
  int i;
  float mean;

  for(i = 0; i < Data_Size; i++){
    //sum
    total = total + number[i];
  }
  mean = total / Data_Size;
  return(mean);
}

float StdDev(float *number,float mean){
  int i;
  float SD=0;

  for(i = 0; i < Data_Size; i++){
    SD += pow(number[i] - mean, 2);
  }
  SD = sqrt(SD / Data_Size);
  return(SD);
}

float  Median(float *number){

  float median;
  median = number[Data_Size/2];
  return(median);
}

float  IQR(float *number){

float IQR,Q1,Q3;
  Q1 = number[Data_Size/4];
  Q3 = number[Data_Size/2 + Data_Size/4];
  IQR = Q3 - Q1;
return(IQR);
}

float  Skew(float *number, float mean, float SD){
  int i;
  float sum=0;
  float skew;
  for(i=0;i<Data_Size;i++){
    sum+=number[i]-pow(mean,3);
  }
  skew=(1/pow(SD,3))*(1/mean)*sum;

return(skew);
}

float Range(float* minmax){
  float range;

  range = minmax[1]-minmax[0];
  return(range);
}

void PrintStat(float* minmax,float mean,float SD,float Variance,float median,float iqr,float skew,float range){
  printf("Min: %f \tMax: %f\n",minmax[0],minmax[1]);
  printf("Mean:%f\n",mean);
  printf("Standard Deviation:%f\n",SD);
  printf("Variance:%f\n",Variance);
  printf("Median:%f\n",median);
  printf("IQR:%f\n",iqr);
  printf("Skew:%f\n",skew);
  printf("Range:%f\n",range);
}

int main(int argc, char** argv)
{
    FILE  *fp;
    float *number;
    float *flip;
    float mean, newMean;
    float *minmax;
	float *newMinMax;
    float SD, newSD;
    float median, newMedian;
    float iqr, newIQR;
    float Variance, newVariance;
    float skew, newSkew;
    float range, newRange;
	int bitFlipIndex;
	int i;
  float RMSE;
  float PSNR;
  float MAE;
  float SEM; //Standard Error of the mean
  float MSE;
  float L2_Norm;
  float L2_Inf;
  fp = fopen ("../vx.dat2", "r");


    number=(float *)malloc(Data_Size*sizeof(float));
    flip =(float *)malloc(Data_Size*sizeof(float));

    fread(number,sizeof(float),Data_Size,fp);
  //  fread(flip,sizeof(float),Data_Size,fp)
    fclose(fp);
	//printf("\nBeginning baseline calculation ...\n");
    minmax=MinMax(number);
    mean=Mean(number);
    SD=StdDev(number,mean);
    Variance=pow(SD,2);
    median=Median(number);
    iqr=IQR(number);
    skew=Skew(number,mean,SD);
    range=Range(minmax);
    //PrintStat(minmax,mean,SD,Variance,median,iqr,skew,range);
	//printf("Beginning Fault Injection...\n");

	//printf("Beginning trial %s.", argv[1]);
	srand((unsigned int)*argv[1]);
	bitFlipIndex = rand() % (Data_Size * 32 + 1);
	//printf("Value before injection: %f    ", number[bitFlipIndex / 32]);
	float dataFlip = number[bitFlipIndex / 32];
	long long* ptr = (long long*)&dataFlip;
	long long tmp = (*ptr ^ (0x1L << (bitFlipIndex/ Data_Size)));
	float *newVal = (float*)&tmp;
	number[bitFlipIndex / 32] = *newVal;

  //*flip=*number;
  for(i=0;i<Data_Size;i++){
  flip[i] = number[i];
  }

  //printf("Value after injection: %f\n", number[bitFlipIndex / 32]);
	newMinMax = MinMax(number);
	newMean = Mean(number);
	newSD = StdDev(number, newMean);
	newIQR = IQR(number);
	newMedian = Median(number);
	newVariance = pow(newSD, 2);
	newSkew = Skew(number, newMean, newSD);

	//printing to test file
	//printf("-------------------------------------\n");
	printf("%f,%f,%f,%f,%f,%f,%f,%f\n", newMinMax[0], newMinMax[1], newMean ,newSD,newIQR, newMedian, newVariance, newSkew);
	printf("%f,%f,%f,%f,%f,%f,%f,%f\n", newMinMax[0] - minmax[0], newMinMax[1] - minmax[1], newMean - mean, newSD - SD, newIQR - iqr, newMedian - median, newVariance - Variance, newSkew - skew);
	//printf("-------------------------------------\n");

	// Reset bit
	number[bitFlipIndex / 32] = (float)((unsigned int)number[bitFlipIndex / 32] ^ (1 << (bitFlipIndex - 1)));
	//printf("Finishing trial %s.\n", argv[1]);
  RMSE=RMSE_Calc(number,flip);
  PSNR=20*log10(minmax[1]/RMSE);
  MAE=MAE_Calc(number,flip);
  SEM=SD/sqrtf(Data_Size);
  MSE=pow(RMSE,2);
  L2_Norm=L2_Norm_Calc(number,flip);
  L2_Inf=L2_Inf_Calc(number);
  printf("0,%f,%f,%f,%f,%f,%f,%f\n",RMSE,PSNR,MAE,SEM,MSE,L2_Norm,L2_Inf);
    return 0;
}
