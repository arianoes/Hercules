//
//  LoadingFilesDataBogota.c
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "LoadingFilesDataBogota.h"


double** loadFilesData (void) {

    FILE *fileA = fopen("/u/sciteam/rianoesc/bogotadatabase/FilesPlanesNameHercules.in", "r");
    
    int i;
    int j;
    int k;
    
    int NPlanes2read;
    fscanf(fileA,"%i",&NPlanes2read);
    
    PlanesData *InputPlanesData = malloc(sizeof(PlanesData)*NPlanes2read);
    
    
    for(i=0; i<NPlanes2read; i++){
        int nameFileLen;
        fscanf(fileA,"%i",&nameFileLen);
        InputPlanesData[i].namePlanes=malloc(sizeof(char)*(nameFileLen+1));
        fscanf(fileA,"%s %lf %lf %lf %lf",InputPlanesData[i].namePlanes,&InputPlanesData[i].IDplane,&InputPlanesData[i].DepthInputPlane,&InputPlanesData[i].PlaneInitialPosition,&InputPlanesData[i].PlaneFinalPosition);
    }
    
    double **OuputData;
    int rows = NPlanes2read;
    int cols = 4;
    // output matrix memory allocation
    OuputData = (double **)malloc(rows*sizeof(double*));
    for(j=0; j<NPlanes2read; j++)
    {
        OuputData[j]=(double *)malloc(cols*sizeof(double));
    }
    
    for(k=0; k<NPlanes2read; k++)
    {
        OuputData[k][0]=InputPlanesData[k].IDplane;
        OuputData[k][1]=InputPlanesData[k].DepthInputPlane;
        OuputData[k][2]=InputPlanesData[k].PlaneInitialPosition;
        OuputData[k][3]=InputPlanesData[k].PlaneFinalPosition;
    }
    
    // clean up
    fclose(fileA);
    for(i=0; i<NPlanes2read; i++)
    {
        free(InputPlanesData[i].namePlanes);
    }
    free(InputPlanesData);
    InputPlanesData=NULL;
    
    return OuputData;
}
