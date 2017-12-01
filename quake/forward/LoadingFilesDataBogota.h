//
//  LoadingFilesDataBogota.h
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef LoadingFilesDataBogota_h
#define LoadingFilesDataBogota_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define sizeplane     330418
#define NPlanesBogota 82

// Variables definition

typedef struct
{
    char   *namePlanes;
    double IDplane;
    double DepthInputPlane;
} PlanesData;

typedef struct
{
    double DepthPlane[NPlanesBogota];
    double InitialPos[NPlanesBogota];
    double FinalPos[NPlanesBogota];
} PlanesFilesData;

typedef struct
{
    float Vp[sizeplane][NPlanesBogota];
    float Vs[sizeplane][NPlanesBogota];
    float rho[sizeplane][NPlanesBogota];
} Planes;


// Function prototype
int loadFilesData (PlanesFilesData *filesData);

#endif /* LoadingFilesDataBogota_h */
