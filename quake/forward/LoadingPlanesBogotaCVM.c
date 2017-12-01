#include "LoadingPlanesBogotaCVM.h"


float* loadPlanesCVMBogota (void) {
    
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
        fscanf(fileB,"%s %lf %lf",InputPlanesName[i].namePlanesCVM,&InputPlanesName[i].IDplaneCVM,&InputPlanesName[i].DepthInputPlaneCVM);
        //printf("%s\n",InputPlanesName[i].namePlanesCVM);
    }
    
    fclose (fileB);
    
    // Load data from planes files in a single structure
    CVMPlane * planeData = malloc(sizeof(CVMPlane)*sizeplane*NPlanesBogota);
    
    for (j=0; j<NPlanesBogota;j++)
    {
        FILE *fileC = fopen(InputPlanesName[j].namePlanesCVM,"r");
        if (fileC == NULL){
            printf("Error reading file: %s\n",InputPlanesName[j].namePlanesCVM);
        }
        for (k=0; k<sizeplane;k++)
        {
            int posFile = k + (sizeplane * j);
            fscanf(fileC,"%f %f %f\n", &planeData[posFile].Vp, &planeData[posFile].Vs, &planeData[posFile].rho);
            //printf("%f %f %f\n", planeData[posFile].Vp, planeData[posFile].Vs, planeData[posFile].rho);
        }
        fclose(fileC);
    }
    
    
    float *outputArray;
    int rows = NPlanesBogota*sizeplane;
    int cols = 3;
    outputArray = (float *)malloc(rows*cols*sizeof(float));
    for(l=0; l<cols; l++)
    {
        for (o=0; o<rows; o++)
        {
            int offset = (l * rows) + o;
            
            if (l==0) {
                outputArray[offset]=planeData[o].Vp;
                //initial position = 0; final position = rows - 1
                
            }
            if (l==1){
                outputArray[offset]=planeData[o].Vs;
                //initial position = rows; final position = (2*rows) - 1
            }
            if (l==2){
                outputArray[offset]=planeData[o].rho;
                //initial position = (2*rows); final position = (3*rows) - 1
            }
            
        }
    }
    //printf("%lf\n",outputArray[0]);            //Vp IDdepth = 0;
    //printf("%lf\n",outputArray[rows - 1]);     //Vp IDdepth = 82;
    //printf("%lf\n",outputArray[rows]);         //Vs IDdepth = 0;
    //printf("%lf\n",outputArray[(2*rows) - 1]); //Vs IDdepth = 82;
    //printf("%lf\n",outputArray[(2*rows)]);     //Rho IDdepth = 0;
    //printf("%lf\n",outputArray[(3*rows) - 1]); //Rho IDdepth = 82;
    
    // clean up
    for(i=0; i<CVM_NPlanes2read; i++)
    {
        free(InputPlanesName[i].namePlanesCVM);
    }
    free(InputPlanesName);
    free(planeData);

    return outputArray;
}
