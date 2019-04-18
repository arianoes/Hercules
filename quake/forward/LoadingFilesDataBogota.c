//
//  LoadingFilesDataBogota.c
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "LoadingFilesDataBogota.h"


int loadFilesData (PlanesFilesData *filesData) {

    FILE *fileA = fopen("/Users/Andrea/Desktop/CVMBogota/bogotadatabaseV2/FilesPlanesName.in", "r");
    if (fileA == NULL){
      printf("Error in file opening in function loadFilesDataBogota\n"); 
    }    
    int i;
    int j;

    int NPlanes2read;
    fscanf(fileA,"%i",&NPlanes2read);
    PlanesData *InputPlanesData = malloc(sizeof(PlanesData)*NPlanes2read);
    for(i=0; i<NPlanes2read; i++){
        int nameFileLen;
        fscanf(fileA,"%i",&nameFileLen);
        InputPlanesData[i].namePlanes=malloc(sizeof(char)*(nameFileLen+1));
        fscanf(fileA,"%s %lf %lf",InputPlanesData[i].namePlanes,&InputPlanesData[i].IDplane,&InputPlanesData[i].DepthInputPlane);
    }
    fclose(fileA);
    
    for(j=0; j<NPlanes2read; j++)
        {
            filesData->DepthPlane[j]=InputPlanesData[j].DepthInputPlane;
            filesData->InitialPos[j]=sizeplane*j;
            filesData->FinalPos[j]=(sizeplane*(j+1))-1;
            //printf("%lf %lf %lf\n",filesData->DepthPlane[j],filesData->InitialPos[j],filesData->FinalPos[j]);
        }
    
    // clean up
    for(i=0; i<NPlanes2read; i++)
    {
        free(InputPlanesData[i].namePlanes);
    }
    free(InputPlanesData);
    
    return 0;
}
