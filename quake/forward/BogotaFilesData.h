//
//  BogotaFilesData.h
//  CVMBogotaHercules
//
//  Created by Andrea camila Riaño escandon  on 11/13/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#ifndef BogotaFilesData_h
#define BogotaFilesData_h

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
    double ID;
    double Depth;
    double ni;
    double nj;
} EntryFiles;  // This structure stores the name of the planes available and the corresponding ID and Depth

double** Bogota_Files_Data(void);

#endif /* BogotaFilesData_h */
