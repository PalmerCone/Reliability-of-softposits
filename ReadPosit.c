//#ifdef SOFTPOSIT_EXACT 
//#undef SOFTPOSIT_EXACT
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

   for(int i = 0; i < Data_Size; i++){
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

  for(int i = 0; i < Data_Size; i++){
    //sum
    total = total + number[i];
  }
  mean = total / Data_Size;
  return(mean);
}

float StdDev(float *number,float mean){
  int i;
  float SD=0;

  for(int i = 0; i < Data_Size; i++){
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
    posit32_t *number;
    posit32_t *flip;
	char testName[30];
    posit32_t mean, newMean;
    posit32_t *minmax;
	posit32_t *newMinMax;
    posit32_t SD, newSD;
    posit32_t median, newMedian;
    posit32_t iqr, newIQR;
    posit32_t Variance, newVariance;
    posit32_t skew, newSkew;
    posit32_t range, newRange;
	int bitFlipIndex;
	unsigned char *value;
	int i;
  float RMSE;
  float PSNR;
  float MAE;
  float SEM; //Standard Error of the mean
  float MSE;
  float L2_Norm;
  float L2_Inf;
    fp = fopen ("../vx.dat2", "r");


    number = (posit32_t *)malloc( Data_Size * sizeof(posit32_t) );
    flip = (posit32_t *)malloc( Data_Size * sizeof(posit32_t) );

    fread( number,sizeof(float),Data_Size,fp);
  //  fread(flip,sizeof(float),Data_Size,fp)
    fclose(fp);
	//printf("\nBeginning baseline calculation ...\n");
    minmax=convertFloatToP32(MinMax(convertP32ToFloat(number)));
    mean=convertFloatToP32(Mean(convertP32ToFloat(number)));
    SD=convertFloatToP32(StdDev(convertP32ToFloat(number),convertP32ToFloat(Mean)));
    Variance=convertFloatToP32(pow(convertP32ToFloat(SD),2));
    median=convertFloatToP32(Median(convertP32ToFloat(number)));
    iqr=convertFloatToP32(IQR(convertP32ToFloat(number)));
    skew=convertFloatToP32(Skew(convertP32ToFloat(number),convertP32ToFloat(mean),convertP32ToFloat(SD)));
    range=convertFloatToP32(Range(convertP32ToFloat(minmax)));
    //PrintStat(minmax,mean,SD,Variance,median,iqr,skew,range);
	//printf("Beginning Fault Injection...\n");

	//printf("Beginning trial %s.", argv[1]);
	srand((uint32_t)*argv[1]);
	bitFlipIndex = rand() % (Data_Size * 32 + 1);
	printf("Value before injection: %f    ", number[bitFlipIndex / 32]);
	posit32_t dataFlip = number[bitFlipIndex / 32];
	uint32_t* ptr = (uint32_t*) &dataFlip;
	uint32_t tmp = (*ptr ^ (0x1L << (bitFlipIndex/ Data_Size)));
	posit32_t *newVal = (posit32_t*)&tmp; // this may change
	number[bitFlipIndex / 32] = *newVal;

  //*flip=*number;
  for(i=0;i<Data_Size;i++){
    flip[i] = number[i];
  }

   printf("Value after injection: %f\n", number[bitFlipIndex / 32]);
	newMinMax = convertFloatToP32(MinMax(convertP32ToFloat(number)));
	newMean = convertFloatToP32(Mean(convertP32ToFloat(number)));
	newSD = convertFloatToP32(StdDev(convertP32ToFloat(number), convertP32ToFloat(mean), convertP32ToFloat(SD)));
	newIQR = convertFloatToP32(IQR(convertP32ToFloat(number)));
    newMedian=convertFloatToP32(Median(convertP32ToFloat(number)));
	newVariance = convertFloatToP32(pow(newSD, 2));
	newSkew = convertFloatToP32(Skew(convertP32ToFloat(number), convertP32ToFloat(newMean),convertP32ToFloat(newSD)));
  printf("%f,%f,%f,%f,%f,%f,%f,%f\n", newMinMax[0], newMinMax[1], newMean ,newSD,newIQR, newMedian, newVariance, newSkew);
 printf("%f,%f,%f,%f,%f,%f,%f,%f\n", newMinMax[0] - minmax[0], newMinMax[1] - minmax[1], newMean - mean, newSD - SD, newIQR - iqr, newMedian - median, newVariance - Variance, newSkew - skew);

	//printing to test file
	//sprintf(testName, "test_%s.txt", argv[1]);
	//fp2 = fopen(testName, "w");
	/*printf("-------------------------------------\n");
	printf("Min: %f\nMax: %f\nMean: %f\nStandard Deviation: %f\nIQR: %f\n", convertP32ToFloat(newMinMax[0]), convertP32ToFloat(newMinMax[1]), convertP32ToFloat(newMean) , convertP32ToFloat(newSD), convertP32ToFloat(newIQR));
	printf("**Differences**\nMin: %f\nMax: %f\nMean: %f\nStandard Deviation: %f\nIQR: %f\n", convertP32ToFloat(newMinMax[0]) - convertP32ToFloat(minmax[0]), convertP32ToFloat(newMinMax[1]) - convertP32ToFloat(minmax[1]), convertP32ToFloat(newMean) - convertP32ToFloat(mean), convertP32ToFloat(newSD) - convertP32ToFloat(SD), convertP32ToFloat(newIQR) - convertP32ToFloat(iqr));
	printf("-------------------------------------\n");*/
	//fclose(fp2);

	// Reset bit
	//number[bitFlipIndex / 32] = ui32_to_p32(p32_to_ui32([bitFlipIndex / 32]) ^ (1 << (bitFlipIndex - 1)));
    //printf("Finishing trial %s.\n", argv[1]);
    tmp = (*ptr ^ (0x1L << (bitFlipIndex/ Data_Size)));
	*newVal = (posit32_t*)&tmp; // this may change
	number[bitFlipIndex / 32] = *newVal;
    
    printf("Value after reinjection: %f\n", number[bitFlipIndex / 32]);
  RMSE= RMSE_Calc(convertP32ToFloat(number),convertP32ToFloat(flip));
  PSNR=20*log10(convertP32ToFloat(minmax[1])/RMSE);
  MAE=MAE_Calc(convertP32ToFloat(number),convertP32ToFloat(flip));
  SEM=convertP32ToFloat(SD)/sqrtf(Data_Size);
  MSE=pow(RMSE,2);
  L2_Norm=L2_Norm_Calc(convertP32ToFloat(number),convertP32ToFloat(flip));
  L2_Inf=L2_Inf_Calc(convertP32ToFloat(number));
   printf("0,%f,%f,%f,%f,%f,%f,%f\n",RMSE,PSNR,MAE,SEM,MSE,L2_Norm,L2_Inf);
    return 0;
}
//#endif