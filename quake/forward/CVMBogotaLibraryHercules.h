//
//  CVMBogotaLibraryHercules.h
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef CVMBogotaLibraryHercules_h
#define CVMBogotaLibraryHercules_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LoadingFilesDataBogota.h"
#include "cvm.h"  // strcuture cvmpayload_t is defined in this header file in hercules.

//sizeplane and NPlanesBogota are defined in LoadingPlanesBogotaCVM.h
#define sizeplane     330418
#define NPlanesBogota 82 //number of input planes
#define GridSpacing 200  //grid spacing from input plane
#define nx 653           //number of grig points in x-dir, input plane
#define ny 506           //number of grig points in y-dir, input plane

// This structure must be comented in hercules
/*
typedef struct
{
    float Vp;
    float Vs;
    float rho;
} cvmpayload_t;
*/

typedef struct
{
    double diffValue[NPlanesBogota];
    int position[NPlanesBogota];
} diffDepth;

typedef struct
{
    double xcoor[nx];
    int xpos[nx];
    double ycoor[ny];
    int ypos[ny];
} diffCoor;

typedef struct
{
    int IdPosZeroXcoor[1];
    int IdPosZeroYcoor[1];
} ZeroDiffCoor;

typedef struct
{
    int IdClosestNegXcoor[1];
    int IdClosestNegYcoor[1];
} ClosestNegDiff;

typedef struct
{
    int IdClosestPosXcoor[1];
    int IdClosestPosYcoor[1];
} ClosestPosDiff;

typedef struct
{
    double fxy1a;
    double fxy1b;
    double fxy2a;
    double fxy2b;
    double c1;
    double c2;
} BilinearInterpConstants;

typedef struct
{
    double xd;
    double yd;
    double zd;
} TrilinearInterpConstants;

/* -------------------------------------------------------------------------------------------*/
/* Functions propotypes */
/* -------------------------------------------------------------------------------------------*/
double bilinear(double x, double y,
                double x1, double y1, double x2, double y2,
                double q11, double q21, double q12, double q22);
double get_lat(double x, double y);
double get_lon(double x, double y);
double* convert(double x, double y, double z);

int DiffVectorDepth(int NData, double DataVector[NData], double DataPointInterest, diffDepth *diff);
int DiffVectorCoor(int NDatax, int NDatay, double DataVectorx[NDatax], double DataVectory[NDatay], double DataPointInterestx,double DataPointInteresty, diffCoor *diff);
int DiffEqualZeroDepth(int nData, diffDepth diff);
int DiffEqualZeroCoor(int nDatax,int nDatay, diffCoor diff, ZeroDiffCoor *diffId);
int ClosestNegDepth(int nData, diffDepth diff);
int ClosestNegCoor(int nDatax,int nDatay, diffCoor diff, ClosestNegDiff *diffId);
int ClosestPosDepth(int nData, diffDepth diff);
int ClosestPosCoor(int nDatax,int nDatay, diffCoor diff, ClosestPosDiff *diffId);

int BilinearInterpCons(double x1, double x2, double y1,double y2, double x, double y,BilinearInterpConstants *BilinearCons);
int TrilinearInterCons(double x0, double x1, double y0, double y1, double z0, double z1, double x, double y, double z, TrilinearInterpConstants *TrilinearCons);

int CVMBogota(double XcoorPoint,double YcoorPoint,double DepthPoint,PlanesFilesData filesData,float* dataPlaneOutput, cvmpayload_t* payload);


#endif /* CVMBogotaLibraryHercules_h */
