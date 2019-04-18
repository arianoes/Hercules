//
//  CVMBogotaLibraryHercules.c
//  CVMBogotaH
//
//  Created by Andrea camila Riaño escandon  on 11/15/17.
//  Copyright © 2017 Andrea camila Riaño escandon . All rights reserved.
//

#include "CVMBogotaLibraryHercules.h"

// Funtions to convert from XY to LL ------------------------------------------------------------------------------------------------------
double bilinear(double x, double y,
                double x1, double y1, double x2, double y2,
                double q11, double q21, double q12, double q22)
{
    double p = (x2 - x1) * (y2 - y1);
    double f1 = (q11 / p) * (x2 - x) * (y2 - y);
    double f2 = (q21 / p) * (x - x1) * (y2 - y);
    double f3 = (q12 / p) * (x2 - x) * (y - y1);
    double f4 = (q22 / p) * (x - x1) * (y - y1);
    return f1 + f2 + f3 + f4;
}

double get_lat(double x, double y) {
    
    // Latitudes of Bogota region corners (130x101)--ARGGIS
    return bilinear(x,y,
                    0, 0, 130402.370855, 101000.008112,
                    4.460303, 3.799435,
                    5.216928, 4.555367);
}

double get_lon(double x, double y) {
    
    // Longitudes of Bogota region corners (130x101)--ARGGIS
    return bilinear(x,y,
                    0, 0, 130402.370855, 101000.008112,
                    -74.702151, -73.7294453,
                    -74.192142, -73.218927);
}

double* convert(double x, double y, double z) {
    
    double lat, lon;
    double* coordinatesVector =  (double*)malloc(3*sizeof(double));
    
    
    // 130.5km in x direction
    if(x<0 || x>130400){
        fprintf(stderr,"Invalid x, value must be between 0 and max x-coordinate: %lf\n",x);
        //return 1;
    }
    
    // 101km in y direction
    if(y<0 || y>101000){
        fprintf(stderr,"Invalid y, value must be between 0 and max y-coordinate: %lf\n",y);
        //return 1;
    }
    
    lat = get_lat(x,y);
    lon = get_lon(x,y);
    
    coordinatesVector[0]=lon;
    coordinatesVector[1]=lat;
    coordinatesVector[2]=z;
    //printf("%.11lf %.11lf\n",lat,lon);
    
    return coordinatesVector;
    //return 0;
}


// Functions to interpolate ------------------------------------------------------------------------------------------------------
int DiffVectorDepth(int NData, double DataVector[NData], double DataPointInterest, diffDepth *diffDepth)
{
    // allocating numDepthsAvailable rows
    int i;
    for (i=0; i<NData; i++)
    {
        // allocating 2 columns in each row
        diffDepth->diffValue[i] = DataVector[i] - DataPointInterest; //difference
        //printf("%lf %lf %lf\n",diff[i][0],DepthsAvailable[i], DepthPointInterest); //debug point to check array diff
        diffDepth->position[i] =i; // position in the vector
        //printf("%lf %lf \n",diff[i][0],diff[i][1]); //debug point to check array diff
    }
    return 0;
}


int DiffVectorCoor(int NDatax, int NDatay, double DataVectorx[NDatax], double DataVectory[NDatay], double DataPointInterestx,double DataPointInteresty, diffCoor *diffCoor)
{
    // allocating numDepthsAvailable rows
    int i;
    for (i=0; i<NDatax; i++)
    {
        // allocating 2 columns in each row
        diffCoor->xcoor[i] = DataVectorx[i] - DataPointInterestx; //difference
        //printf("%lf %lf %lf\n",diff[i][0],DepthsAvailable[i], DepthPointInterest); //debug point to check array diff
        diffCoor->xpos[i]=i; // position in the vector
        //printf("%lf %lf \n",diff[i][0],diff[i][1]); //debug point to check array diff
    }
    for (i=0; i<NDatay; i++)
    {
        // allocating 2 columns in each row
        diffCoor->ycoor[i] = DataVectory[i] - DataPointInteresty; //difference
        //printf("%lf %lf %lf\n",diff[i][0],DepthsAvailable[i], DepthPointInterest); //debug point to check array diff
        diffCoor->ypos[i]=i; // position in the vector
        //printf("%lf %lf \n",diff[i][0],diff[i][1]); //debug point to check array diff
    }
    return 0;
}


// -------------------------------------------------------------------------------------------------------

int DiffEqualZeroDepth(int nData, diffDepth diffDepth)
{
    int idPosZero;
    double minval;
    int i=0;
    for (i=0; i<nData; i++)
    {
        // printf("%lf\n",diff[i][0]); debug checkpoint
        if (diffDepth.diffValue[i]==0)
        {
            minval    =diffDepth.diffValue[i];
            idPosZero =diffDepth.position[i];
            break;
        }
        else
        {
            minval    =-1;
            idPosZero =-1;
        }
    }
    //printf("%lf %i \n",minval, idPosZero);
    return idPosZero;
}

int DiffEqualZeroCoor(int nDatax,int nDatay, diffCoor diffCoor, ZeroDiffCoor *diffId)
{
    double minvalx;
    double minvaly;
    int i;
    for (i=0; i<nDatax; i++)
    {
        // printf("%lf\n",diff[i][0]); debug checkpoint
        if (diffCoor.xcoor[i]==0)
        {
            minvalx=diffCoor.xcoor[i];
            diffId->IdPosZeroXcoor[0] = diffCoor.xpos[i];
            break;
        }
        else
        {
            minvalx=-1;
            diffId->IdPosZeroXcoor[0] =-1;
        }
    }
    for (i=0; i<nDatay; i++)
    {
        // printf("%lf\n",diff[i][0]); debug checkpoint
        if (diffCoor.ycoor[i]==0)
        {
            minvaly=diffCoor.ycoor[i];
            diffId->IdPosZeroYcoor[0] = diffCoor.ypos[i];
            break;
        }
        else
        {
            minvaly=-1;
            diffId->IdPosZeroYcoor[0] =-1;
        }
    }
    //printf("%lf %i \n",minval, idPosZero);
    return 0;
}

// -------------------------------------------------------------------------------------------------------

int ClosestNegDepth(int nData, diffDepth diffDepth)
{
    
    double minvalneg=-1000000000000;
    double closestvalNegative;
    int idClosestNeg;
    int i;
    for (i=0; i<nData; i++)
    {
        if (diffDepth.diffValue[i]<0 && diffDepth.diffValue[i]>minvalneg)
        {
            closestvalNegative=diffDepth.diffValue[i];
            minvalneg = closestvalNegative;
            idClosestNeg = diffDepth.position[i];
        }
    }
    //printf("%lf %i \n",closestvalNegative, idClosestNeg);
    return idClosestNeg;
}

int ClosestNegCoor(int nDatax,int nDatay, diffCoor diffCoor, ClosestNegDiff *diffId)
{
    
    double minvalnegx=-1000000000000;
    double minvalnegy=-1000000000000;
    double closestvalNegative;
    int i;
    for (i=0; i<nDatax; i++)
    {
        if (diffCoor.xcoor[i]<0 && diffCoor.xcoor[i]>minvalnegx)
        {
            closestvalNegative=diffCoor.xcoor[i];
            minvalnegx = closestvalNegative;
            diffId->IdClosestNegXcoor[0] = diffCoor.xpos[i];
        }
    }
    
    for (i=0; i<nDatay; i++)
    {
        if (diffCoor.ycoor[i]<0 && diffCoor.ycoor[i]>minvalnegy)
        {
            closestvalNegative=diffCoor.ycoor[i];
            minvalnegy = closestvalNegative;
            diffId->IdClosestNegYcoor[0] = diffCoor.ypos[i];
        }
    }
    //printf("%lf %i \n",closestvalNegative, idClosestNeg);
    return 0;
}

// -------------------------------------------------------------------------------------------------------

int ClosestPosDepth(int nData, diffDepth diffDepth)
{
    
    double minvalpos=10000000000000;
    double closestminvalPositive;
    int idClosestPos;
    int i;
    for (i=0; i<nData; i++)
    {
        if (diffDepth.diffValue[i]>0 && diffDepth.diffValue[i]<minvalpos)
        {
            closestminvalPositive=diffDepth.diffValue[i];
            minvalpos=closestminvalPositive;
            idClosestPos =diffDepth.position[i];
        }
    }
    
    //printf("%lf %i \n",closestminvalPositive, idClosestPos);
    return idClosestPos;
}

int ClosestPosCoor(int nDatax,int nDatay, diffCoor diffCoor, ClosestPosDiff *diffId)
{
    
    double minvalposx=10000000000000;
    double minvalposy=10000000000000;
    double closestminvalPositive;
    int i;
    for (i=0; i<nDatax; i++)
    {
        if (diffCoor.xcoor[i]>0 && diffCoor.xcoor[i]<minvalposx)
        {
            closestminvalPositive=diffCoor.xcoor[i];
            minvalposx=closestminvalPositive;
            diffId->IdClosestPosXcoor[0] = diffCoor.xpos[i];
        }
    }
    for (i=0; i<nDatay; i++)
    {
        if (diffCoor.ycoor[i]>0 && diffCoor.ycoor[i]<minvalposy)
        {
            closestminvalPositive=diffCoor.ycoor[i];
            minvalposy=closestminvalPositive;
            diffId->IdClosestPosYcoor[0] = diffCoor.ypos[i];
        }
    }
    //printf("%lf %i \n",closestminvalPositive, idClosestPos);
    return 0;
}

// -------------------------------------------------------------------------------------------------------
int BilinearInterpCons(double x1, double x2, double y1,double y2, double x, double y,BilinearInterpConstants *BilinearCons)
{
    BilinearCons->fxy1a = ((x2-x)/(x2-x1));
    BilinearCons->fxy1b =((x-x1)/(x2-x1));
    BilinearCons->fxy2a =((x2-x)/(x2-x1));
    BilinearCons->fxy2b =((x-x1)/(x2-x1));
    BilinearCons->c1=((y2-y)/(y2-y1));
    BilinearCons->c2=((y-y1)/(y2-y1));
    return 0;
}

// -------------------------------------------------------------------------------------------------------
int TrilinearInterCons(double x0, double x1, double y0, double y1, double z0, double z1, double x, double y, double z, TrilinearInterpConstants *TrilinearCons)
{
    TrilinearCons->xd = (x-x0)/(x1-x0);
    TrilinearCons->yd = (y-y0)/(y1-y0);
    TrilinearCons->zd = (z-z0)/(z1-z0);
    return 0;
}

// function starts here -----------------------------------------------------------------------------------------------------
int CVMBogota(double XcoorPoint,double YcoorPoint,double DepthPoint,PlanesFilesData filesData,float* dataPlaneOutput, cvmpayload_t* payload){
    // A polygon inside the CVM region
    double distXcornerc11 = 0;     //if =0 it means simulation area starts in the x origin of the CVM region
    double distYcornerc11 = 30000;     //if =0 it means simulation area starts in the y origin of the CVM region
    
    // position planes data vector
    int IniPosVp=0;
    int IniPosVs=NPlanesBogota*sizeplane; //67074854; if NPlanes=203
    int IniPosRho=NPlanesBogota*sizeplane*2; //134149708; if NPlanes=203
    
    // Convert input coordinates from XY to LL
    //double* coordinatesVector = convert(x,y,DepthPoint);
    
    // Vector for results
    float CVMResult[3];
    // Define coordinates vector for interpolation --------------------------------------------------------------------------------------------------------
    double xcoorvector[nx];
    double ycoorvector[ny];
    xcoorvector[0]=0;
    ycoorvector[0]=0;
    int i;
    for (i=1; i<nx;i++)
    {
        xcoorvector[i]=xcoorvector[i-1]+GridSpacing;
        //printf("%lf\n", xcoorvector[i]);  // debug chekpoint
        
    }
    for (i=1; i<ny;i++)
    {
        ycoorvector[i]=ycoorvector[i-1]+GridSpacing;
        //printf("%lf\n", ycoorvector[i]);  // debug chekpoint
    }
    
    // Find the planes or plane that are/is needed to interpolate depending on the depth given as input --------------------------------------------------------------------------------------------------------
    /* 1. Find the difference vector between the depth of the point and available depths */

    diffDepth diff;
    DiffVectorDepth(NPlanesBogota,filesData.DepthPlane,DepthPoint,&diff);
    
    /* 2. find if the min dif == max dif == 0 or if min dif =~ max dif */
    int IndexZero;
    int IndexClosestNeg;
    int IndexClosestPos;
    IndexZero = DiffEqualZeroDepth(NPlanesBogota, diff); //find index for diff=0, if result = -1 there's no diff=0
    IndexClosestNeg = ClosestNegDepth(NPlanesBogota,diff);
    IndexClosestPos = ClosestPosDepth(NPlanesBogota,diff);
    
    /* 3. Define if I need to perform a 2D interp or a 3D interp. If IndexZero=~0 2D interp, 3D interp otherwise. */
    //------------------------------------------------------------------------------------------If IndexZero == -1 means that there is not a plane with the input Depth
    // 2D INTERPD
    if (IndexZero != -1) // 2D INTERPOLATION STARTS HERE ------------------------------------------------------
    {
        /* 1. find diff between the x-coor and y-coor of the point of interest and the plane grid coordinates */
        diffCoor diffCoordinates;
        DiffVectorCoor(nx,ny,xcoorvector,ycoorvector,XcoorPoint+distXcornerc11,YcoorPoint+distYcornerc11, &diffCoordinates);
        
        /* 2. find if the min dif == max dif == 0 or if min dif =~ max dif */
        ZeroDiffCoor ZeroDiffCoorPos;
        ClosestNegDiff ClosestNegDiffPosition;
        ClosestPosDiff ClosestPosDiffPosition;
        DiffEqualZeroCoor(nx, ny, diffCoordinates, &ZeroDiffCoorPos);
        ClosestNegCoor(nx, ny, diffCoordinates,&ClosestNegDiffPosition);
        ClosestPosCoor(nx, ny, diffCoordinates,&ClosestPosDiffPosition);
        int IndexZeroX=ZeroDiffCoorPos.IdPosZeroXcoor[0];
        int IndexZeroY=ZeroDiffCoorPos.IdPosZeroYcoor[0];
        int IndexClosestNegX=ClosestNegDiffPosition.IdClosestNegXcoor[0];
        int IndexClosestNegY=ClosestNegDiffPosition.IdClosestNegYcoor[0];
        int IndexClosestPosX=ClosestPosDiffPosition.IdClosestPosXcoor[0];
        int IndexClosestPosY=ClosestPosDiffPosition.IdClosestPosYcoor[0];
    
        
        /* 3. 2D INTERPOLATION */
    
        if (IndexZeroX != -1 && IndexZeroY != -1) // Point in the grid mesh
        {
            int posData = IndexZeroX + (nx*IndexZeroY);            //Position of the point in the plane
            int posVector_i=filesData.InitialPos[IndexZero];  //Position corresponding to the begining of each plane
            CVMResult[0] = dataPlaneOutput[IniPosVp+(posData+posVector_i)];
            CVMResult[1] = dataPlaneOutput[IniPosVs+(posData+posVector_i)];
            CVMResult[2] = dataPlaneOutput[IniPosRho+(posData+posVector_i)];
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
        }
            
        else if (IndexZeroX == -1 && IndexZeroY == -1) // Bilinear interp
        {
            
            BilinearInterpConstants BilinearCons;
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            int posVector_i=filesData.InitialPos[IndexZero];
            
            int posData11=IndexClosestNegX+(nx*IndexClosestNegY);
            int posData12=IndexClosestPosX+(nx*IndexClosestNegY);
            int posData21=IndexClosestNegX+(nx*IndexClosestPosY);
            int posData22=IndexClosestPosX+(nx*IndexClosestPosY);
            float Vp_q11=dataPlaneOutput[IniPosVp+posVector_i+posData11];
            float Vp_q12=dataPlaneOutput[IniPosVp+posVector_i+posData12];
            float Vp_q21=dataPlaneOutput[IniPosVp+posVector_i+posData21];
            float Vp_q22=dataPlaneOutput[IniPosVp+posVector_i+posData22];
            float Vs_q11=dataPlaneOutput[IniPosVs+posVector_i+posData11];
            float Vs_q12=dataPlaneOutput[IniPosVs+posVector_i+posData12];
            float Vs_q21=dataPlaneOutput[IniPosVs+posVector_i+posData21];
            float Vs_q22=dataPlaneOutput[IniPosVs+posVector_i+posData22];
            float rho_q11=dataPlaneOutput[IniPosRho+posVector_i+posData11];
            float rho_q12=dataPlaneOutput[IniPosRho+posVector_i+posData12];
            float rho_q21=dataPlaneOutput[IniPosRho+posVector_i+posData21];
            float rho_q22=dataPlaneOutput[IniPosRho+posVector_i+posData22];
            double x1=xcoorvector[IndexClosestNegX];
            double x2=xcoorvector[IndexClosestPosX];
            double y1=ycoorvector[IndexClosestNegY];
            double y2=ycoorvector[IndexClosestPosY];
            double fxy1;
            double fxy2;
            
            BilinearInterpCons(x1,x2,y1,y2,x,y,&BilinearCons);
            
            fxy1=(BilinearCons.fxy1a*Vp_q11)+(BilinearCons.fxy1b*Vp_q21);
            fxy2=(BilinearCons.fxy2a*Vp_q12)+(BilinearCons.fxy2b*Vp_q22);
            CVMResult[0]=(BilinearCons.c1*fxy1)+(BilinearCons.c2*fxy2);
           
            fxy1=(BilinearCons.fxy1a*Vs_q11)+(BilinearCons.fxy1b*Vs_q21);
            fxy2=(BilinearCons.fxy2a*Vs_q12)+(BilinearCons.fxy2b*Vs_q22);
            CVMResult[1]=(BilinearCons.c1*fxy1)+(BilinearCons.c2*fxy2);
            
            fxy1=(BilinearCons.fxy1a*rho_q11)+(BilinearCons.fxy1b*rho_q21);
            fxy2=(BilinearCons.fxy2a*rho_q12)+(BilinearCons.fxy2b*rho_q22);
            CVMResult[2]=(BilinearCons.c1*fxy1)+(BilinearCons.c2*fxy2);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
        }
        else if (IndexZeroX != -1 && IndexZeroY == -1) // linear interp x on grid
        {
            double y=YcoorPoint+distYcornerc11;
            int posVector_i=filesData.InitialPos[IndexZero];
            
            int posData1=(IndexClosestNegY*nx)+(IndexZeroX);
            int posData2=(IndexClosestPosY*nx)+(IndexZeroX);
            double y1=ycoorvector[IndexClosestNegY];
            double y2=ycoorvector[IndexClosestPosY];
            float Vp_x1=dataPlaneOutput[IniPosVp+posVector_i+posData1];
            float Vp_x2=dataPlaneOutput[IniPosVp+posVector_i+posData2];
            float Vs_x1=dataPlaneOutput[IniPosVs+posVector_i+posData1];
            float Vs_x2=dataPlaneOutput[IniPosVs+posVector_i+posData2];
            float Rho_x1=dataPlaneOutput[IniPosRho+posVector_i+posData1];
            float Rho_x2=dataPlaneOutput[IniPosRho+posVector_i+posData2];
            
            CVMResult[0]=Vp_x1+(y-y1)*((Vp_x2-Vp_x1)/(y2-y1));
            CVMResult[1]=Vs_x1+(y-y1)*((Vs_x2-Vs_x1)/(y2-y1));
            CVMResult[2]=Rho_x1+(y-y1)*((Rho_x2-Rho_x1)/(y2-y1));
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
        }
        else if (IndexZeroX == -1 && IndexZeroY != -1) // linear interp y on grid
        {
            double x=XcoorPoint+distXcornerc11;
            int posVector_i=filesData.InitialPos[IndexZero];
            
            int posData1=IndexZeroY+(nx*IndexClosestNegX);
            int posData2=IndexZeroY+(nx*IndexClosestPosX);
            double x1=xcoorvector[IndexClosestNegX];
            double x2=xcoorvector[IndexClosestPosX];
            float Vp_y1=dataPlaneOutput[IniPosVp+posVector_i+posData1];
            float Vp_y2=dataPlaneOutput[IniPosVp+posVector_i+posData2];
            float Vs_y1=dataPlaneOutput[IniPosVs+posVector_i+posData1];
            float Vs_y2=dataPlaneOutput[IniPosVs+posVector_i+posData2];
            float Rho_y1=dataPlaneOutput[IniPosRho+posVector_i+posData1];
            float Rho_y2=dataPlaneOutput[IniPosRho+posVector_i+posData2];

            CVMResult[0]=Vp_y1+(x-x1)*((Vp_y2-Vp_y1)/(x2-x1));
            CVMResult[1]=Vs_y1+(x-x1)*((Vs_y2-Vs_y1)/(x2-x1));
            CVMResult[2]=Rho_y1+(x-x1)*((Rho_y2-Rho_y1)/(x2-x1));
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
        }
    }
    else // 3D INTERPOLATION STARTS HERE ------------------------------------------------------
    {
        // 1. find diff between the x-coor and y-coor of the point of interest and the plane grid coordinates
        diffCoor diffCoordinates;
        DiffVectorCoor(nx,ny,xcoorvector,ycoorvector,XcoorPoint+distXcornerc11,YcoorPoint+distYcornerc11, &diffCoordinates);
        
        // 2. find if the min dif == max dif == 0 or if min dif =~ max dif
        ZeroDiffCoor ZeroDiffCoorPos;
        ClosestNegDiff ClosestNegDiffPosition;
        ClosestPosDiff ClosestPosDiffPosition;
        DiffEqualZeroCoor(nx, ny, diffCoordinates, &ZeroDiffCoorPos);
        ClosestNegCoor(nx, ny, diffCoordinates,&ClosestNegDiffPosition);
        ClosestPosCoor(nx, ny, diffCoordinates,&ClosestPosDiffPosition);
        int IndexZeroX=ZeroDiffCoorPos.IdPosZeroXcoor[0];
        int IndexZeroY=ZeroDiffCoorPos.IdPosZeroYcoor[0];
        int IndexClosestNegX=ClosestNegDiffPosition.IdClosestNegXcoor[0];
        int IndexClosestNegY=ClosestNegDiffPosition.IdClosestNegYcoor[0];
        int IndexClosestPosX=ClosestPosDiffPosition.IdClosestPosXcoor[0];
        int IndexClosestPosY=ClosestPosDiffPosition.IdClosestPosYcoor[0];
        
        
        // 3. 3D INTERPOLATION
        
        if (IndexZeroX == -1 && IndexZeroY == -1) // Trilinear interp
        {
            TrilinearInterpConstants TrilinearCons;
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            double z=DepthPoint;
            int LowerPlanePosVector_i=filesData.InitialPos[IndexClosestPos];
            int UpperPlanePosVector_i=filesData.InitialPos[IndexClosestNeg];
            
            int posData00=IndexClosestNegX+(nx*IndexClosestNegY);
            int posData01=IndexClosestPosX+(nx*IndexClosestNegY);
            int posData10=IndexClosestNegX+(nx*IndexClosestPosY);
            int posData11=IndexClosestPosX+(nx*IndexClosestPosY);
            
            double x0=xcoorvector[IndexClosestNegX];
            double x1=xcoorvector[IndexClosestPosX];
            double y0=ycoorvector[IndexClosestNegY];
            double y1=ycoorvector[IndexClosestPosY];
            double z0=filesData.DepthPlane[IndexClosestNeg];
            double z1=filesData.DepthPlane[IndexClosestPos];
            
            float Vp_q00Lower=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData00];
            float Vp_q01Lower=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData01];
            float Vp_q10Lower=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData10];
            float Vp_q11Lower=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData11];
            float Vs_q00Lower=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData00];
            float Vs_q01Lower=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData01];
            float Vs_q10Lower=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData10];
            float Vs_q11Lower=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData11];
            float Rho_q00Lower=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData00];
            float Rho_q01Lower=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData01];
            float Rho_q10Lower=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData10];
            float Rho_q11Lower=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData11];
            
            float Vp_q00Upper=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData00];
            float Vp_q01Upper=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData01];
            float Vp_q10Upper=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData10];
            float Vp_q11Upper=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData11];
            float Vs_q00Upper=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData00];
            float Vs_q01Upper=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData01];
            float Vs_q10Upper=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData10];
            float Vs_q11Upper=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData11];
            float Rho_q00Upper=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData00];
            float Rho_q01Upper=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData01];
            float Rho_q10Upper=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData10];
            float Rho_q11Upper=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData11];

            TrilinearInterCons(x0,x1,y0,y1,z0,z1,x,y,z,&TrilinearCons);
            
            float Vp_c00=(Vp_q00Lower*(1-TrilinearCons.xd))+(Vp_q10Lower*TrilinearCons.xd);
            float Vp_c01=(Vp_q00Upper*(1-TrilinearCons.xd))+(Vp_q10Upper*TrilinearCons.xd);
            float Vp_c10=(Vp_q01Lower*(1-TrilinearCons.xd))+(Vp_q11Lower*TrilinearCons.xd);
            float Vp_c11=(Vp_q01Upper*(1-TrilinearCons.xd))+(Vp_q11Upper*TrilinearCons.xd);
            float Vp_c0=(Vp_c00*(1-TrilinearCons.yd))+(Vp_c10*TrilinearCons.yd);
            float Vp_c1=(Vp_c01*(1-TrilinearCons.yd))+(Vp_c11*TrilinearCons.yd);
            CVMResult[0]=(Vp_c0*(1-TrilinearCons.zd))+(Vp_c1*TrilinearCons.zd);
            
            float Vs_c00=(Vs_q00Lower*(1-TrilinearCons.xd))+(Vs_q10Lower*TrilinearCons.xd);
            float Vs_c01=(Vs_q00Upper*(1-TrilinearCons.xd))+(Vs_q10Upper*TrilinearCons.xd);
            float Vs_c10=(Vs_q01Lower*(1-TrilinearCons.xd))+(Vs_q11Lower*TrilinearCons.xd);
            float Vs_c11=(Vs_q01Upper*(1-TrilinearCons.xd))+(Vs_q11Upper*TrilinearCons.xd);
            float Vs_c0=(Vs_c00*(1-TrilinearCons.yd))+(Vs_c10*TrilinearCons.yd);
            float Vs_c1=(Vs_c01*(1-TrilinearCons.yd))+(Vs_c11*TrilinearCons.yd);
            CVMResult[1]=(Vs_c0*(1-TrilinearCons.zd))+(Vs_c1*TrilinearCons.zd);
            
            float Rho_c00=(Rho_q00Lower*(1-TrilinearCons.xd))+(Rho_q10Lower*TrilinearCons.xd);
            float Rho_c01=(Rho_q00Upper*(1-TrilinearCons.xd))+(Rho_q10Upper*TrilinearCons.xd);
            float Rho_c10=(Rho_q01Lower*(1-TrilinearCons.xd))+(Rho_q11Lower*TrilinearCons.xd);
            float Rho_c11=(Rho_q01Upper*(1-TrilinearCons.xd))+(Rho_q11Upper*TrilinearCons.xd);
            float Rho_c0=(Rho_c00*(1-TrilinearCons.yd))+(Rho_c10*TrilinearCons.yd);
            float Rho_c1=(Rho_c01*(1-TrilinearCons.yd))+(Rho_c11*TrilinearCons.yd);
            CVMResult[2]=(Rho_c0*(1-TrilinearCons.zd))+(Rho_c1*TrilinearCons.zd);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            
        }
        else //if (IndexZeroX != -1 && IndexZeroY != -1) // linear interp along z
        {
            double z=DepthPoint;
            int LowerPlanePosVector_i=filesData.InitialPos[IndexClosestPos];
            int UpperPlanePosVector_i=filesData.InitialPos[IndexClosestNeg];
            double z0=filesData.DepthPlane[IndexClosestNeg];
            double z1=filesData.DepthPlane[IndexClosestPos];
            
            if (IndexZeroX != -1 && IndexZeroY != -1) // x and y are on the grid
            {
                int posData=IndexZeroX+(nx*IndexZeroY); /////////
                float Vp_Fz0=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData];
                float Vp_Fz1=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData];
                float Vs_Fz0=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData];
                float Vs_Fz1=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData];
                float Rho_Fz0=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData];
                float Rho_Fz1=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData];
                
                CVMResult[0]=Vp_Fz0+(((Vp_Fz1-Vp_Fz0)/(z1-z0))*(z-z0));
                CVMResult[1]=Vs_Fz0+(((Vs_Fz1-Vs_Fz0)/(z1-z0))*(z-z0));
                CVMResult[2]=Rho_Fz0+(((Rho_Fz1-Rho_Fz0)/(z1-z0))*(z-z0));

            }
            else if (IndexZeroX != -1 && IndexZeroY == -1) // x is on the grid
            {
                int posData1=(IndexClosestNegY*nx)+(IndexZeroX);
                int posData2=(IndexClosestPosY*nx)+(IndexZeroX);

                float Vp_Fz0=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData1];
                float Vp_Fz1=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData2];
                float Vs_Fz0=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData1];
                float Vs_Fz1=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData2];
                float Rho_Fz0=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData1];
                float Rho_Fz1=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData2];
                
                CVMResult[0]=Vp_Fz0+(((Vp_Fz1-Vp_Fz0)/(z1-z0))*(z-z0));
                CVMResult[1]=Vs_Fz0+(((Vs_Fz1-Vs_Fz0)/(z1-z0))*(z-z0));
                CVMResult[2]=Rho_Fz0+(((Rho_Fz1-Rho_Fz0)/(z1-z0))*(z-z0));
            }
            else if (IndexZeroX == -1 && IndexZeroY != -1) // y is on the grid
            {
                int posData1=IndexZeroY+(nx*IndexClosestNegX);
                int posData2=IndexZeroY+(nx*IndexClosestPosX);
                
                float Vp_Fz0=dataPlaneOutput[IniPosVp+LowerPlanePosVector_i+posData1];
                float Vp_Fz1=dataPlaneOutput[IniPosVp+UpperPlanePosVector_i+posData2];
                float Vs_Fz0=dataPlaneOutput[IniPosVs+LowerPlanePosVector_i+posData1];
                float Vs_Fz1=dataPlaneOutput[IniPosVs+UpperPlanePosVector_i+posData2];
                float Rho_Fz0=dataPlaneOutput[IniPosRho+LowerPlanePosVector_i+posData1];
                float Rho_Fz1=dataPlaneOutput[IniPosRho+UpperPlanePosVector_i+posData2];
                
                CVMResult[0]=Vp_Fz0+(((Vp_Fz1-Vp_Fz0)/(z1-z0))*(z-z0));
                CVMResult[1]=Vs_Fz0+(((Vs_Fz1-Vs_Fz0)/(z1-z0))*(z-z0));
                CVMResult[2]=Rho_Fz0+(((Rho_Fz1-Rho_Fz0)/(z1-z0))*(z-z0));
            }
    //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
        }
    }
    payload->Vp=CVMResult[0];
    payload->Vs=CVMResult[1];
    payload->rho=CVMResult[2]*1000; // density in kg/m3
    //printf("%.11lf %.11lf %lf %lf %lf %lf\n", coordinatesVector[0], coordinatesVector[1], coordinatesVector[2],CVMResult[0],CVMResult[1],CVMResult[2]*1000);
    //printf("%lf %lf %lf %.11lf %.11lf %lf %lf %lf %lf\n", XcoorPoint, XcoorPoint, DepthPoint, coordinatesVector[0], coordinatesVector[1], coordinatesVector[2],payload->Vp,payload->Vs,payload->rho);
    //printf("%lf %lf %lf %lf %lf %lf\n", XcoorPoint, YcoorPoint, DepthPoint ,payload->Vp,payload->Vs,payload->rho);

    if (CVMResult[0] == 0 || CVMResult[1] == 0 || CVMResult[2] == 0)
    {
        printf("%s\n", "CVM ERROR");
        return -1; //error
    }
    return 0;
}


