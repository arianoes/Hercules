//
//  BogotaFilesData.c
//  CVMBogotaHercules
//
//  Created by Andrea camila Riaño escandon  on 11/13/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "BogotaFilesData.h"

double** Bogota_Files_Data(void)
{
    // The following lines of code read a text file containing the path of each plane --------------------------------------------------------------------------------------------------------
    // allocating numDepthsAvailable rows
    double** FilesData =  (double**)malloc(nPlanesBogota*sizeplane*sizeof(double*));
    FILE *filePlanes = fopen("/u/sciteam/rianoesc/bogotadatabase/FilesPlanesNameHercules.in","r");
    int i;
    int entryCount;
    fscanf(filePlanes, "%i", &entryCount);
    EntryFiles *entriesFiles = malloc(sizeof(EntryFiles)*entryCount); // for 82 entries structs
    
    for (i=0; i<entryCount;i++)
    {
        FilesData[i]=(double *)malloc(sizeof(double)*4);
        int nameLen;
        fscanf(filePlanes,"%i",&nameLen);
        entriesFiles[i].name = malloc (sizeof(char)*(nameLen+1));
        fscanf(filePlanes,"%s %lf %lf %lf %lf", entriesFiles[i].name, &entriesFiles[i].ID, &entriesFiles[i].Depth, &entriesFiles[i].ni, &entriesFiles[i].nj);
        //printf("%i. %s %i %lf\n",i+1,entries[i].name,entries[i].ID, entries[i].Depth);  // debug chekpoint
        FilesData[i][1]=entriesFiles[i].ID;
        FilesData[i][2]=entriesFiles[i].Depth;
        FilesData[i][3]=entriesFiles[i].ni;
        FilesData[i][4]=entriesFiles[i].nj;

        //printf("%lf %lf %lf %lf\n",FilesData[i][1],FilesData[i][2],FilesData[i][3],FilesData[i][4]);  // debug chekpoint
    }
    fclose(filePlanes); // Clean up
    for (i=0; i<entryCount;i++)
    {
        free(entriesFiles[i].name);
    }
    free(entriesFiles);
    entriesFiles=NULL;
    return FilesData;
}
