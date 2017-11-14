//
//  CVMBogotaLibraryHercules.h
//  CVMBogotaHercules
//
//  Created by Andrea camila Riaño escandon  on 11/13/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef CVMBogotaLibraryHercules_h
#define CVMBogotaLibraryHercules_h

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "cvm.h"         // strcuture cvmpayload_t is defined in this header file in hercules.

#define sizeplane 330418 //This variable changes if the size of the plane is not 653*506
#define GridSpacing 200  //grid spacing from input plane
#define nx 653           //number of grig points in x-dir, input plane
#define ny 506           //number of grig points in y-dir, input plane
#define nPlanesBogota 82

/*
typedef struct
{
    float Vp;
    float Vs;
    float rho;
} cvmpayload_t;
*/

/* -------------------------------------------------------------------------------------------*/
/* Functions propotypes */
/* -------------------------------------------------------------------------------------------*/
double bilinear(double x, double y,
                double x1, double y1, double x2, double y2,
                double q11, double q21, double q12, double q22);
double get_lat(double x, double y);
double get_lon(double x, double y);
double* convert(double x, double y, double z);
double** DiffVector(int NData, double DataVector[NData], double DataPointInterest);
double DiffEqualZero(int nData, double** diff );
double ClosestNeg(int nData, double** diff );
double ClosestPos(int nData, double** diff );
double interp2(int Nsizeplane, double* VectorData,double* xcoorvector,double* ycoorvector,double x,double y, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax);
double interp3(int ndepths ,int Nsizeplane, double* VectorDataLower,double* VectorDataUpper,double* xcoorvector,double* ycoorvector,double x,double y, double z, int IndexClosestNeg, int IndexClosestPos, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax, double depthVector[ndepths]);
int CVMBogota(double XcoorPoint,double YcoorPoint,double DepthPoint,double** FilesData,double** dataPlaneOutput, cvmpayload_t* payload);

#endif /* CVMBogotaLibraryHercules_h */
