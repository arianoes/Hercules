//
//  LoadingFilesDataBogota.c
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "LoadingFilesDataBogota.h"


double* loadFilesData (void) {

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
        fscanf(fileA,"%s %lf %lf",InputPlanesData[i].namePlanes,&InputPlanesData[i].IDplane,&InputPlanesData[i].DepthInputPlane);
    }
    
    double *OuputData;
    int rows = NPlanes2read;
    int cols = 3;
    OuputData = (double *)malloc(rows*cols*sizeof(double));
    for(j=0; j<cols; j++)
    {
        for (k=0; k<rows; k++)
        {
            int offset = (j * rows) + k;
            
            if (j==0) {
                OuputData[offset]=InputPlanesData[k].DepthInputPlane;
                //initial position = 0; final position = rows - 1
                
            }
            if (j==1){
                OuputData[offset]= sizeplane*k; //InputPlanesData[k].PlaneInitialPosition;
                //initial position = rows; final position = (2*rows) - 1
            }
            if (j==2){
                OuputData[offset]=(sizeplane*(k+1))-1;//InputPlanesData[k].PlaneFinalPosition;
                //initial position = (2*rows); final position = (3*rows) - 1
            }
        }
    }
    
    //printf("%lf\n",OuputData[0]);            //DepthInputPlane IDdepth = 0;
    //printf("%lf\n",OuputData[rows - 1]);     //DepthInputPlane IDdepth = 82;
    //printf("%lf\n",OuputData[rows]);         //PlaneInitialPosition IDdepth = 0;
    //printf("%lf\n",OuputData[(2*rows) - 1]); //PlaneInitialPosition IDdepth = 82;
    //printf("%lf\n",OuputData[(2*rows)]);     //PlaneFinalPosition IDdepth = 0;
    //printf("%lf\n",OuputData[(3*rows) - 1]); //PlaneFinalPosition IDdepth = 82;
    

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
