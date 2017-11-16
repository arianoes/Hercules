//
//  LoadingPlanesBogotaCVM.c
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "LoadingPlanesBogotaCVM.h"


float** loadPlanesCVMBogota (void) {

    FILE *fileB = fopen("/u/sciteam/rianoesc/bogotadatabase/FilesPlanesNameHercules.in", "r");
    
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
    //printf("%lu\n",sizeof(CVMPlane)*sizeplane*NPlanesBogota); // 8 bytes * 7 properties * NPlanes planes * sizeplane data/property --> debug chekpoint
    //printf("%lu\n",sizeof(float*));
    //printf("%lu\n",sizeof(float));
    //printf("%lu\n",NPlanesBogota*sizeplane*sizeof(float)*3*sizeof(float));
    
    for (j=0; j<NPlanesBogota;j++)
    {
        FILE *fileC = fopen(InputPlanesName[j].namePlanesCVM,"r");
        //printf("%s\n",InputPlanesName[j].namePlanesCVM);
        for (k=0; k<sizeplane;k++)
        {
            int posFile = k + (sizeplane * j);
            fscanf(fileC,"%f %f %f\n", &planeData[posFile].Vp, &planeData[posFile].Vs, &planeData[posFile].Ro);
        }
        fclose(fileC);
    }
    
    // output matrix memory allocation
    float **outputArray;
    //float *outputArray;
    int rows = NPlanesBogota*sizeplane;
    int cols = 3;
    outputArray = (float **)malloc(rows*sizeof(float*));
    for(l=0; l<rows; l++)
    {
        outputArray[l]=(float *)malloc(cols*sizeof(float));
    }
    
    //store structure data in an ouput arrray.
    for (o=0; o<rows;o++)
    {
        outputArray[o][0]=planeData[o].Vp;
        outputArray[o][1]=planeData[o].Vs;
        outputArray[o][2]=planeData[o].Ro;
        //printf("%lf %lf %lf\n",outputArray[o][0],outputArray[o][1],outputArray[o][2]);
    }
    
    
    /*
     outputArray = (float *)malloc(rows*cols*sizeof(float));
     int m;
     int mm =0;
     for(l=0; l<cols; l++)
     {
     for (m=0; l<rows; m++)
     {
     int offset= l * cols + m;
     outputArray[offset]=planeData[mm].Vp;
     outputArray[offset+NPlanesBogota*sizeplane]=planeData[mm].Vs;
     outputArray[offset+(NPlanesBogota*sizeplane*2)]=planeData[mm].Ro;
     mm=mm+1;
     //printf("%i %i %i %i\n",offset,offset+NPlanesBogota*sizeplane,offset+(NPlanesBogota*sizeplane*2),mm-1);
     }
     }
     */
    
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
