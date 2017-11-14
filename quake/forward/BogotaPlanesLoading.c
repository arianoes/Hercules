//
//  BogotaPlanesLoading.c
//  CVMBogotaHercules
//
//  Created by Andrea camila Riaño escandon  on 11/13/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "BogotaPlanesLoading.h"

double** Bogota_Planes_Loading(void)
{
    // allocating numDepthsAvailable rows
    double** dataPlaneOutput =  (double**)malloc(nPlanesBogota*sizeplane*sizeof(double*));
    int nPlanes=0;
    CVMPlane *planeData = malloc(sizeof(CVMPlane)*1);
    // The following lines of code read a text file containing the path of each plane --------------------------------------------------------------------------------------------------------
    FILE *filePlanes = fopen("/u/sciteam/rianoesc/bogotadatabase/FilesPlanesNameHercules.in","r");
    int i;
    int entryCount;
    fscanf(filePlanes, "%i", &entryCount);
    Entry *entries = malloc(sizeof(Entry)*entryCount); // for 82 entries structs
    
    for (i=0; i<entryCount;i++)
    {
        int nameLen;
        fscanf(filePlanes,"%i",&nameLen);
        entries[i].name = malloc (sizeof(char)*(nameLen+1));
        fscanf(filePlanes,"%s %i %lf %lf %lf", entries[i].name, &entries[i].ID, &entries[i].Depth, &entries[i].ni, &entries[i].nj);
        //printf("%i. %s %i %lf\n",i+1,entries[i].name,entries[i].ID, entries[i].Depth);  // debug chekpoint
    }
    fclose(filePlanes); // Clean up
    
    for (i=0; i<entryCount;i++)
    {
        //printf("%lu",sizeof(CVMPlane)*entryCount); // 8 bytes * 7 properties * NPlanes planes * 120701 data/property --> debug chekpoint
        FILE *file = fopen(entries[i].name,"r");
        //printf("%s\n",entries[i].name);
        int j;
        for (j=0; j<sizeplane;j++)
        {
            int posFile = j + (sizeplane * i);
            fscanf(file,"%lf %lf %lf %lf %lf %lf %lf", &planeData[nPlanes].Xcoor[posFile], &planeData[nPlanes].Ycoor[posFile],&planeData[nPlanes].Elevation[posFile], &planeData[nPlanes].Depth[posFile], &planeData[nPlanes].Vp[posFile], &planeData[nPlanes].Vs[posFile], &planeData[nPlanes].Ro[posFile]);
            dataPlaneOutput[posFile]=(double *)malloc(sizeof(double)*7);
            dataPlaneOutput[posFile][0]=planeData[nPlanes].Xcoor[posFile];
            dataPlaneOutput[posFile][1]=planeData[nPlanes].Ycoor[posFile];
            dataPlaneOutput[posFile][2]=planeData[nPlanes].Elevation[posFile];
            dataPlaneOutput[posFile][3]=planeData[nPlanes].Depth[posFile];
            dataPlaneOutput[posFile][4]=planeData[nPlanes].Vp[posFile];
            dataPlaneOutput[posFile][5]=planeData[nPlanes].Vs[posFile];
            dataPlaneOutput[posFile][6]=planeData[nPlanes].Ro[posFile];
            //printf("%lf %lf %lf %lf %lf %lf %lf\n", planeData[i].Xcoor[j], planeData[i].Ycoor[j], planeData[i].Elevation[j], planeData[i].Depth[j], planeData[i].Vp[j], planeData[i].Vs[j], planeData[i].Ro[j]);
            //printf("%lf %lf %lf %lf %lf %lf %lf\n",dataPlaneOutput[posFile][0], dataPlaneOutput[posFile][1], dataPlaneOutput[posFile][2], dataPlaneOutput[posFile][3], dataPlaneOutput[posFile][4], dataPlaneOutput[posFile][5], dataPlaneOutput[posFile][6]);
        }
        // Clean up
        fclose(file);
    }
    for (i=0; i<entryCount;i++)
    {
        free(entries[i].name);
    }
    free(entries);
    entries=NULL;
    return dataPlaneOutput;
}
