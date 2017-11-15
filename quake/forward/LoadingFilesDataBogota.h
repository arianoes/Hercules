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

// Variables definition

typedef struct
{
    char   *namePlanes;
    double IDplane;
    double DepthInputPlane;
    double PlaneInitialPosition;
    double PlaneFinalPosition;
    
} PlanesData;

// Function prototype
double** loadFilesData (void);

#endif /* LoadingFilesDataBogota_h */
