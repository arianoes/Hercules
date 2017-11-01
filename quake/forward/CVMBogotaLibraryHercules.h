//
//  CVMBogotaLibraryHercules.h
//  CVMBogota 
//
//  Created by Andrea camila Riaño escandon  on 10/30/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef CVMBogotaLibraryHercules_h
#define CVMBogotaLibraryHercules_h

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "cvm.h"  // strcuture cvmpayload_t is defined in this header file.

#endif /* CVMBogotaLibraryHercules_h */

#define sizeplane 330418 //This variable changes if the size of the plane is not 653*506
#define spacing 200      //grid spacing from input plane
#define nx 653           //number of grig points in x-dir, input plane
#define ny 506           //number of grig points in y-dir, input plane

typedef struct
{
    char *name;
    int  ID;
    double Depth;
} Entry;  // This structure stores the name of the planes available and the corresponding ID and Depth

typedef struct
{
    double xcoor;
    double ycoor;
} Coordinates;  // This structure stores the name of the planes available and the corresponding ID and Depth

typedef struct
{
    double Xcoor[sizeplane];
    double Ycoor[sizeplane];
    double Elevation[sizeplane];
    double Depth[sizeplane];
    double Vp[sizeplane];
    double Vs[sizeplane];
    double Ro[sizeplane];
} CVMPlane; // This structure stores the values of the CVM for each plane.
// Size = 8 bytes * 7 parameters * sizeplane * number of planes

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
double interp2(int Nsizeplane, double VectorData[Nsizeplane],double* xcoorvector,double* ycoorvector,double x,double y, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax);
double interp3(int ndepths ,int Nsizeplane, double VectorDataLower[Nsizeplane],double VectorDataUpper[Nsizeplane],double* xcoorvector,double* ycoorvector,double x,double y, double z, int IndexClosestNeg, int IndexClosestPos, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax, double depthVector[ndepths]);
int CVMBogota(double XcoorPoint,double YcoorPoint,double DepthPoint,cvmpayload_t* payload)
