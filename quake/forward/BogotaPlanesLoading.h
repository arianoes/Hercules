//
//  BogotaPlanesLoading.h
//  CVMBogotaHercules
//
//  Created by Andrea camila Riaño escandon  on 11/13/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef BogotaPlanesLoading_h
#define BogotaPlanesLoading_h

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define sizeplane 330418 //This variable changes if the size of the plane is not 653*506
#define GridSpacing 200  //grid spacing from input plane
#define nx 653           //number of grig points in x-dir, input plane
#define ny 506           //number of grig points in y-dir, input plane
#define nPlanesBogota 82 //number of planes

typedef struct
{
    char *name;
    int  ID;
    double Depth;
    double ni;
    double nj;
} Entry;  // This structure stores the name of the planes available and the corresponding ID and Depth

typedef struct
{
    double Xcoor[sizeplane*nPlanesBogota];
    double Ycoor[sizeplane*nPlanesBogota];
    double Elevation[sizeplane*nPlanesBogota];
    double Depth[sizeplane*nPlanesBogota];
    double Vp[sizeplane*nPlanesBogota];
    double Vs[sizeplane*nPlanesBogota];
    double Ro[sizeplane*nPlanesBogota];
} CVMPlane; // This structure stores the values of the CVM for each plane.
// Size = 8 bytes * 7 parameters * sizeplane * number of planes

typedef struct
{
    double Xcoor[sizeplane*nPlanesBogota];
    double Ycoor[sizeplane*nPlanesBogota];
    double Elevation[sizeplane*nPlanesBogota];
    double Depth[sizeplane*nPlanesBogota];
    double Vp[sizeplane*nPlanesBogota];
    double Vs[sizeplane*nPlanesBogota];
    double Ro[sizeplane*nPlanesBogota];
} CVMPlaneOutput; // This structure stores the values of the CVM for each plane.
// Size = 8 bytes * 7 parameters * sizeplane * number of planes

// Funtions prototypes
double** Bogota_Planes_Loading(void);

#endif /* BogotaPlanesLoading_h */
