//
//  LoadingPlanesBogotaCVM.h
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef LoadingPlanesBogotaCVM_h
#define LoadingPlanesBogotaCVM_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define sizeplane     125751
#define NPlanesBogota 64

typedef struct
{
    char   *namePlanesCVM;
    double IDplaneCVM;
    double DepthInputPlaneCVM;
    double PlaneInitialPositionCVM;
    double PlaneFinalPositionCVM;
    
} PlanesName;

typedef struct
{
// This structure stores the values of the CVM for each plane Size = 8 bytes * 7 parameters * sizeplane * number of planes
    float Vp;
    float Vs;
    float Ro;
} CVMPlane;


// Function prototype
float* loadPlanesCVMBogota (void);

#endif /* LoadingPlanesBogotaCVM_h */
