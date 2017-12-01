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

#define sizeplane     330418
#define NPlanesBogota 82

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
    float Vp;
    float Vs;
    float rho;
} CVMPlane;

// Function prototype
float *loadPlanesCVMBogota (void);

#endif /* LoadingPlanesBogotaCVM_h */
