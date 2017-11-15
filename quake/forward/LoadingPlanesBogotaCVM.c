//
//  LoadingPlanesBogotaCVM.c
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "LoadingPlanesBogotaCVM.h"


double** loadPlanesCVMBogota (void) {

    FILE *fileB = fopen("/u/sciteam/rianoesc/bogotadatabase/FilesPlanesName.in", "r");
    
    int i;
    int j;
    int k;
    int l;
    int o;
    
    int CVM_NPlanes2read;
    fscanf(fileB,"%i",&CVM_NPlanes2read);
    
    PlanesName *InputPlanesName = malloc(sizeof(PlanesName)*CVM_NPlanes2read);
   
    for(i=0; i<CVM_NPlanes2read; i++){
        int CVMnameFileLen;
        fscanf(fileB,"%i",&CVMnameFileLen);
        InputPlanesName[i].namePlanesCVM=malloc(sizeof(char)*(CVMnameFileLen+1));
        fscanf(fileB,"%s %lf %lf %lf %lf",InputPlanesName[i].namePlanesCVM,&InputPlanesName[i].IDplaneCVM,&InputPlanesName[i].DepthInputPlaneCVM,&InputPlanesName[i].PlaneInitialPositionCVM,&InputPlanesName[i].PlaneFinalPositionCVM);
        //printf("%s\n",InputPlanesName[i].namePlanesCVM);
    }
    
    fclose (fileB);
    
    // Load data from planes files in a single structure
    CVMPlane *planeData = malloc(sizeof(CVMPlane)*sizeplane*NPlanesBogota);
    
    for (j=0; j<NPlanesBogota;j++)
    {
        FILE *fileC = fopen(InputPlanesName[j].namePlanesCVM,"r");
        //printf("%s\n",InputPlanesName[j].namePlanesCVM);
        for (k=0; k<sizeplane;k++)
        {
            int posFile = k + (sizeplane * j);
            fscanf(fileC,"%lf %lf %lf %lf %lf %lf %lf", &planeData[posFile].Xcoor, &planeData[posFile].Ycoor,&planeData[posFile].Elevation, &planeData[posFile].Depth, &planeData[posFile].Vp, &planeData[posFile].Vs, &planeData[posFile].Ro);
    
        }
        fclose(fileC);
    }
    
    // output matrix memory allocation
    double **outputArray;
    int rows = NPlanesBogota*sizeplane;
    int cols = 7;
    outputArray = (double **)malloc(rows*sizeof(double*));
    for(l=0; l<rows; l++)
    {
        outputArray[l]=(double *)malloc(cols*sizeof(double));
    }
    
    //store structure data in an ouput arrray.
    for (o=0; o<rows;o++)
    {
        outputArray[o][0]=planeData[o].Xcoor;
        outputArray[o][1]=planeData[o].Ycoor;
        outputArray[o][2]=planeData[o].Elevation;
        outputArray[o][3]=planeData[o].Depth;
        outputArray[o][4]=planeData[o].Vp;
        outputArray[o][5]=planeData[o].Vs;
        outputArray[o][6]=planeData[o].Ro;
        //printf("%lf %lf %lf %lf %lf %lf %lf\n",outputArray[o][0],outputArray[o][1],outputArray[o][2],outputArray[o][3],outputArray[o][4],outputArray[o][6],outputArray[o][7]);
    }
    
    // clean up
    for(i=0; i<CVM_NPlanes2read; i++)
    {
        free(InputPlanesName[i].namePlanesCVM);
    }
    free(InputPlanesName);
    InputPlanesName=NULL;
    free(planeData);
    planeData =NULL;
    
    return outputArray;
}
