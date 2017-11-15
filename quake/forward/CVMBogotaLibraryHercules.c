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
    
    
    // 80km in x direction
    if(x<0 || x>130400){
        fprintf(stderr,"Invalid x, value must be between 0 and max x-coordinate: %lf\n",x);
        //return 1;
    }
    
    // 60km in y direction
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
double** DiffVector(int NData, double DataVector[NData], double DataPointInterest)
{
    // allocating numDepthsAvailable rows
    double** diff =  (double**)malloc(NData*sizeof(double*));
    int i;
    for (i=0; i<NData; i++)
    {
        // allocating 2 columns in each row
        diff[i]=(double *)malloc(sizeof(double)*2);
        diff[i][0]= DataVector[i] - DataPointInterest; //difference
        //printf("%lf %lf %lf\n",diff[i][0],DepthsAvailable[i], DepthPointInterest); //debug point to check array diff
        diff[i][1]=i; // position in the vector
        //printf("%lf %lf \n",diff[i][0],diff[i][1]); //debug point to check array diff
    }
    return diff;
}

// -------------------------------------------------------------------------------------------------------

double DiffEqualZero(int nData, double** diff )
{
    int idPosZero;
    double minval;
    int i=0;
    for (i=0; i<nData; i++)
    {
        // printf("%lf\n",diff[i][0]); debug checkpoint
        if (diff[i][0]==0)
        {
            minval    =diff[i][0];
            idPosZero =diff[i][1];
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

// -------------------------------------------------------------------------------------------------------

double ClosestNeg(int nData, double** diff )
{
    
    double minvalneg=-1000000000000;
    double closestvalNegative;
    int idClosestNeg;
    int i;
    for (i=0; i<nData; i++)
    {
        if (diff[i][0]<0 && diff[i][0]>minvalneg)
        {
            closestvalNegative=diff[i][0];
            minvalneg = closestvalNegative;
            idClosestNeg = diff[i][1];
        }
    }
    //printf("%lf %i \n",closestvalNegative, idClosestNeg);
    return idClosestNeg;
}

// -------------------------------------------------------------------------------------------------------

double ClosestPos(int nData, double** diff )
{
    
    double minvalpos=10000000000000;
    double closestminvalPositive;
    int idClosestPos;
    int i;
    for (i=0; i<nData; i++)
    {
        if (diff[i][0]>0 && diff[i][0]<minvalpos)
        {
            closestminvalPositive=diff[i][0];
            minvalpos=closestminvalPositive;
            idClosestPos =diff[i][1];
        }
    }
    
    //printf("%lf %i \n",closestminvalPositive, idClosestPos);
    return idClosestPos;
}

// -------------------------------------------------------------------------------------------------------
double interp2(int Nsizeplane, double* VectorData,double* xcoorvector,double* ycoorvector,double x,double y, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax)
{
    double fxy;
    
    if (method == 1) // Bilinear interpolation
    {
        int posData11=IndexClosestNegX+(NDatax*IndexClosestNegY);
        int posData12=IndexClosestPosX+(NDatax*IndexClosestNegY);
        int posData21=IndexClosestNegX+(NDatax*IndexClosestPosY);
        int posData22=IndexClosestPosX+(NDatax*IndexClosestPosY);
        double x1=xcoorvector[IndexClosestNegX];
        double x2=xcoorvector[IndexClosestPosX];
        double y1=ycoorvector[IndexClosestNegY];
        double y2=ycoorvector[IndexClosestPosY];
        double q11=VectorData[posData11];
        double q12=VectorData[posData12];
        double q21=VectorData[posData21];
        double q22=VectorData[posData22];
        
        
        double fxy1;
        double fxy2;
        double c1;
        double c2;
        
        fxy1 = ((((x2-x)/(x2-x1))*q11)+(((x-x1)/(x2-x1))*q21));
        fxy2 = ((((x2-x)/(x2-x1))*q12)+(((x-x1)/(x2-x1))*q22));
        
        c1=((y2-y)/(y2-y1));
        c2=((y-y1)/(y2-y1));
        
        fxy=(c1*fxy1)+(c2*fxy2);
        
    }
    else if (method == 2) // Linear interpolation
    {
        if (IndexZeroX != -1 && IndexZeroY == -1) // x is on the grid
        {
            int posData1=(IndexClosestNegY*NDatax)+(IndexZeroX);
            int posData2=(IndexClosestPosY*NDatax)+(IndexZeroX);
            double x1=VectorData[posData1];
            double x2=VectorData[posData2];
            double y1=ycoorvector[IndexClosestNegY];
            double y2=ycoorvector[IndexClosestPosY];
            
            fxy=x1+(y-y1)*((x2-x1)/(y2-y1));
        }
        else if (IndexZeroX == -1 && IndexZeroY != -1) // y is on the grid
        {
            int posData1=IndexZeroY+(nx*IndexClosestNegX);
            int posData2=IndexZeroY+(nx*IndexClosestPosX);
            double x1=xcoorvector[IndexClosestNegX];
            double x2=xcoorvector[IndexClosestPosX];
            double y1=VectorData[posData1];
            double y2=VectorData[posData2];
            
            fxy=y1+(x-x1)*((y2-y1)/(x2-x1));
        }
        else
        {
            fxy=NAN;
            printf("%s\n","error in Linear interp, x and y out of boundaries");
        }
    }
    else
    {
        fxy=NAN;
        printf("%s\n","check method value. method = 1 for bilinear interp. or 2 for linear interp.");
    }
    return fxy;
}

// -------------------------------------------------------------------------------------------------------
double interp3(int ndepths ,int Nsizeplane, double* VectorDataLower,double* VectorDataUpper,double* xcoorvector,double* ycoorvector,double x,double y, double z, int IndexClosestNeg, int IndexClosestPos, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax, double depthVector[ndepths])
{
    double fxyz;
    
    if (method == 1) // Trilinear interpolation
    {
        
        int posData00=IndexClosestNegX+(NDatax*IndexClosestNegY);
        int posData01=IndexClosestPosX+(NDatax*IndexClosestNegY);
        int posData10=IndexClosestNegX+(NDatax*IndexClosestPosY);
        int posData11=IndexClosestPosX+(NDatax*IndexClosestPosY);
        
        double x0=xcoorvector[IndexClosestNegX];
        double x1=xcoorvector[IndexClosestPosX];
        double y0=ycoorvector[IndexClosestNegY];
        double y1=ycoorvector[IndexClosestPosY];
        double z0=depthVector[IndexClosestNeg];
        double z1=depthVector[IndexClosestPos];
        
        double q00Lower=VectorDataLower[posData00];
        double q01Lower=VectorDataLower[posData01];
        double q10Lower=VectorDataLower[posData10];
        double q11Lower=VectorDataLower[posData11];
        
        double q00Upper=VectorDataUpper[posData00];
        double q01Upper=VectorDataUpper[posData01];
        double q10Upper=VectorDataUpper[posData10];
        double q11Upper=VectorDataUpper[posData11];
        
        double xd;
        double yd;
        double zd;
        double c00;
        double c01;
        double c10;
        double c11;
        double c0;
        double c1;
        
        xd=(x-x0)/(x1-x0);
        yd=(y-y0)/(y1-y0);
        zd=(z-z0)/(z1-z0);
        
        c00=(q00Lower*(1-xd))+(q10Lower*xd);
        c01=(q00Upper*(1-xd))+(q10Upper*xd);
        c10=(q01Lower*(1-xd))+(q11Lower*xd);
        c11=(q01Upper*(1-xd))+(q11Upper*xd);
        
        c0=(c00*(1-yd))+(c10*yd);
        c1=(c01*(1-yd))+(c11*yd);
        
        fxyz=(c0*(1-zd))+(c1*zd);
        
    }
    else if (method == 2) // Linear interpolation along z-axis
    {
        if (IndexZeroX != -1 && IndexZeroY != -1) // x and y are on the grid
        {
            int posData=IndexZeroX+(NDatax*IndexZeroY); /////////
            double z0=depthVector[IndexClosestNeg];
            double z1=depthVector[IndexClosestPos];
            double Fz0=VectorDataLower[posData];
            double Fz1=VectorDataUpper[posData];
            
            fxyz=Fz0+(((Fz1-Fz0)/(z1-z0))*(z-z0));
        }
        else if (IndexZeroX != -1 && IndexZeroY == -1) // x is on the grid
        {
            int posData1=(IndexClosestNegY*NDatax)+(IndexZeroX);
            int posData2=(IndexClosestPosY*NDatax)+(IndexZeroX);
            double z0=depthVector[IndexClosestNeg];
            double z1=depthVector[IndexClosestPos];
            double Fz0=VectorDataLower[posData1];
            double Fz1=VectorDataUpper[posData2];
            
            fxyz=Fz0+(((Fz1-Fz0)/(z1-z0))*(z-z0));
        }
        else if (IndexZeroX == -1 && IndexZeroY != -1) // y is on the grid
        {
            int posData1=IndexZeroY+(NDatax*IndexClosestNegX);
            int posData2=IndexZeroY+(NDatax*IndexClosestPosX);
            double z0=depthVector[IndexClosestNeg];
            double z1=depthVector[IndexClosestPos];
            double Fz0=VectorDataLower[posData1];
            double Fz1=VectorDataUpper[posData2];
            
            fxyz=Fz0+(((Fz1-Fz0)/(z1-z0))*(z-z0));
        }
        else
        {
            fxyz=NAN;
            printf("%s\n","error in 3d interp, x and y out of boundaries");
        }
    }
    else
    {
        fxyz=NAN;
        printf("%s\n","check method value. method = 1 for trilinear interp. or 2 for linear interp.");
    }
    return fxyz;
}

// function starts here -----------------------------------------------------------------------------------------------------
int CVMBogota(double XcoorPoint,double YcoorPoint,double DepthPoint,double** FilesData,double** dataPlaneOutput, cvmpayload_t* payload)
{
    // A polygon inside the CVM region
    double distXcornerc11 = 0;     //if =0 it means simulation area starts in the x origin of the CVM region
    double distYcornerc11 = 0; //if =0 it means simulation area starts in the y origin of the CVM region
    double x = XcoorPoint + distXcornerc11;
    double y = YcoorPoint + distYcornerc11;
    // Convert input coordinates from XY to LL
    double* coordinatesVector = convert(x,y,DepthPoint);
    
    // Vector for results
    double* CVMResult = (double *)malloc(sizeof(double)*3);
    // Define coordinates vector for interpolation --------------------------------------------------------------------------------------------------------
    double* xcoorvector = (double *)malloc(sizeof(double)*nx);
    double* ycoorvector = (double *)malloc(sizeof(double)*ny);
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
    //double depthVector[nPlanesBogota];
    double* depthVector = (double *)malloc(sizeof(double)*NPlanesBogota);
    for (i=0; i<NPlanesBogota;i++)
    {
        depthVector[i]=FilesData[i][1];
        //printf("%lf \n", FilesData[i][1]);//debug point to check depthvector
        //printf("%lf \n", depthVector[i]); //debug point to check depthvector
    }
    double** diff;
    diff=DiffVector(NPlanesBogota,depthVector,DepthPoint);
    
    /* 2. find if the min dif == max dif == 0 or if min dif =~ max dif */
    int IndexZero;
    int IndexClosestNeg;
    int IndexClosestPos;
    IndexZero = DiffEqualZero(NPlanesBogota,diff); //find index for diff=0, if result = -1 there's no diff=0
    IndexClosestNeg = ClosestNeg(NPlanesBogota,diff);
    IndexClosestPos = ClosestPos(NPlanesBogota,diff);
    
    /* 3. Define if I need to perform a 2D interp or a 3D interp. If IndexZero=~0 2D interp, 3D interp otherwise. */
    //------------------------------------------------------------------------------------------If IndexZero == -1 means that there is not a plane with the input Depth
    // 2D INTERPD
    if (IndexZero != -1) // 2D INTERPOLATION STARTS HERE ------------------------------------------------------
    {
        /* 1. find diff between the x-coor and y-coor of the point of interest and the plane grid coordinates */
        
        double** DiffXcoor;
        double** DiffYcoor;
        DiffXcoor = DiffVector(nx, xcoorvector, XcoorPoint+distXcornerc11);
        DiffYcoor = DiffVector(ny, ycoorvector, YcoorPoint+distYcornerc11);
        
        /* 2. find if the min dif == max dif == 0 or if min dif =~ max dif */
        int IndexZeroX;
        int IndexClosestNegX;
        int IndexClosestPosX;
        IndexZeroX = DiffEqualZero(nx,DiffXcoor); //find index for diff=0, if result = -1 there's no diff=0
        IndexClosestNegX = ClosestNeg(nx,DiffXcoor);
        IndexClosestPosX = ClosestPos(nx,DiffXcoor);
        
        int IndexZeroY;
        int IndexClosestNegY;
        int IndexClosestPosY;
        IndexZeroY = DiffEqualZero(ny,DiffYcoor); //find index for diff=0, if result = -1 there's no diff=0
        IndexClosestNegY = ClosestNeg(ny,DiffYcoor);
        IndexClosestPosY = ClosestPos(ny,DiffYcoor);
        
        /* 3. 2D INTERPOLATION */
        
        if (IndexZeroX != -1 && IndexZeroY != -1) // Point in the grid mesh
        {
            int posData = IndexZeroX + (nx*IndexZeroY);            //Position of the point in the plane
            int posVector_i=FilesData[IndexZero][2];               //Position corresponding to the begining of each plane
            CVMResult[0]= dataPlaneOutput[posData+posVector_i][4]; //planeData[nPlanes].Vp[posData];
            CVMResult[1]= dataPlaneOutput[posData+posVector_i][5]; //planeData[nPlanes].Vs[posData];
            CVMResult[2]= dataPlaneOutput[posData+posVector_i][6]; //planeData[nPlanes].Ro[posData];
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            
        }
        
        else if (IndexZeroX == -1 && IndexZeroY == -1) // Bilinear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            int posVector_i=FilesData[IndexZero][2];
            int posVector_j=FilesData[IndexZero][3];
            double* dataPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            int i;
            int j=0;
            for (i=posVector_i; i<posVector_j+1; i++)
            {
                dataPlaneVp[j]=dataPlaneOutput[i][4];
                dataPlaneVs[j]=dataPlaneOutput[i][5];
                dataPlaneRho[j]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                j=j+1;
            }
            CVMResult[0]=interp2(sizeplane,dataPlaneVp,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx);
            CVMResult[1]=interp2(sizeplane,dataPlaneVs,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx);
            CVMResult[2]=interp2(sizeplane,dataPlaneRho,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            free(dataPlaneVp);
            dataPlaneVp=NULL;
            free(dataPlaneVs);
            dataPlaneVs=NULL;
            free(dataPlaneRho);
            dataPlaneRho=NULL;
        }
        
        else if (IndexZeroX != -1 && IndexZeroY == -1) // linear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            int posVector_i=FilesData[IndexZero][2];
            int posVector_j=FilesData[IndexZero][3];
            double* dataPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            int i;
            int j=0;
            for (i=posVector_i; i<posVector_j+1; i++)
            {
                dataPlaneVp[j]=dataPlaneOutput[i][4];
                dataPlaneVs[j]=dataPlaneOutput[i][5];
                dataPlaneRho[j]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                j=j+1;
            }
            
            CVMResult[0]=interp2(sizeplane,dataPlaneVp,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[1]=interp2(sizeplane,dataPlaneVs,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[2]=interp2(sizeplane,dataPlaneRho,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            free(dataPlaneVp);
            dataPlaneVp=NULL;
            free(dataPlaneVs);
            dataPlaneVs=NULL;
            free(dataPlaneRho);
            dataPlaneRho=NULL;
        }
        
        else if (IndexZeroX == -1 && IndexZeroY != -1) // linear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            int posVector_i=FilesData[IndexZero][2];
            int posVector_j=FilesData[IndexZero][3];
            double* dataPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            int i;
            int j=0;
            for (i=posVector_i; i<posVector_j+1; i++)
            {
                dataPlaneVp[j]=dataPlaneOutput[i][4];
                dataPlaneVs[j]=dataPlaneOutput[i][5];
                dataPlaneRho[j]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                j=j+1;
            }
            CVMResult[0]=interp2(sizeplane,dataPlaneVp,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[1]=interp2(sizeplane,dataPlaneVs,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[2]=interp2(sizeplane,dataPlaneRho,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            free(dataPlaneVp);
            dataPlaneVp=NULL;
            free(dataPlaneVs);
            dataPlaneVs=NULL;
            free(dataPlaneRho);
            dataPlaneRho=NULL;
        }
        // Dealocate memory
        int i;
        for (i=0; i<NPlanesBogota;i++)
        {
            free(diff[i]);
        }
        free(diff);
        diff=NULL;
        for (i=0; i<nx;i++)
        {
            free(DiffXcoor[i]);
        }
        DiffXcoor=NULL;
        free(DiffXcoor);
        for (i=0; i<ny;i++)
        {
            free(DiffYcoor[i]);
        }
        free(DiffYcoor);
        DiffYcoor=NULL;
    }
    else // 3D INTERPOLATION STARTS HERE ------------------------------------------------------
    {
        /* 1. find diff between the x-coor and y-coor of the point of interest and the plane grid coordinates */
        
        double** DiffXcoor;
        double** DiffYcoor;
        DiffXcoor = DiffVector(nx, xcoorvector, XcoorPoint+distXcornerc11);
        DiffYcoor = DiffVector(ny, ycoorvector, YcoorPoint+distYcornerc11);
        
        /* 2. find if the min dif == max dif == 0 or if min dif =~ max dif */
        int IndexZeroX;
        int IndexClosestNegX;
        int IndexClosestPosX;
        IndexZeroX = DiffEqualZero(nx,DiffXcoor); //find index for diff=0, if result = -1 there's no diff=0
        IndexClosestNegX = ClosestNeg(nx,DiffXcoor);
        IndexClosestPosX = ClosestPos(nx,DiffXcoor);
        
        int IndexZeroY;
        int IndexClosestNegY;
        int IndexClosestPosY;
        IndexZeroY = DiffEqualZero(ny,DiffYcoor); //find index for diff=0, if result = -1 there's no diff=0
        IndexClosestNegY = ClosestNeg(ny,DiffYcoor);
        IndexClosestPosY = ClosestPos(ny,DiffYcoor);
        
        
        /* 3. 3D INTERPOLATION */
        
        if (IndexZeroX == -1 && IndexZeroY == -1) // Trilinear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            double z=DepthPoint;
            int LowerPlanePosVector_i=FilesData[IndexClosestPos][2];
            int LowerPlanePosVector_j=FilesData[IndexClosestPos][3];
            int UpperPlanePosVector_i=FilesData[IndexClosestNeg][2];
            int UpperPlanePosVector_j=FilesData[IndexClosestNeg][3];
            double* dataLowerPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataLowerPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataLowerPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            double* dataUpperPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataUpperPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataUpperPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            int i;
            int j=0;
            int k=0;
            for (i=LowerPlanePosVector_i; i<LowerPlanePosVector_j+1; i++)
            {
                dataLowerPlaneVp[j]=dataPlaneOutput[i][4];
                dataLowerPlaneVs[j]=dataPlaneOutput[i][5];
                dataLowerPlaneRho[j]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                j=j+1;
            }
            for (i=UpperPlanePosVector_i; i<UpperPlanePosVector_j+1; i++)
            {
                dataUpperPlaneVp[k]=dataPlaneOutput[i][4];
                dataUpperPlaneVs[k]=dataPlaneOutput[i][5];
                dataUpperPlaneRho[k]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                k=k+1;
            }
            CVMResult[0]=interp3(NPlanesBogota,sizeplane,dataLowerPlaneVp,dataUpperPlaneVp,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx,depthVector);
            CVMResult[1]=interp3(NPlanesBogota,sizeplane,dataLowerPlaneVs,dataUpperPlaneVs,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx,depthVector);
            CVMResult[2]=interp3(NPlanesBogota,sizeplane,dataLowerPlaneRho,dataUpperPlaneRho,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx,depthVector);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            free(dataLowerPlaneVp);
            dataLowerPlaneVp=NULL;
            free(dataLowerPlaneVs);
            dataLowerPlaneVs=NULL;
            free(dataLowerPlaneRho);
            dataLowerPlaneRho=NULL;
            free(dataUpperPlaneVp);
            dataUpperPlaneVp=NULL;
            free(dataUpperPlaneVs);
            dataUpperPlaneVs=NULL;
            free(dataUpperPlaneRho);
            dataUpperPlaneRho=NULL;
        }
        else //if (IndexZeroX != -1 && IndexZeroY != -1) // linear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            double z=DepthPoint;
            int LowerPlanePosVector_i=FilesData[IndexClosestPos][2];
            int LowerPlanePosVector_j=FilesData[IndexClosestPos][3];
            int UpperPlanePosVector_i=FilesData[IndexClosestNeg][2];
            int UpperPlanePosVector_j=FilesData[IndexClosestNeg][3];
            double* dataLowerPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataLowerPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataLowerPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            double* dataUpperPlaneVp = (double *)malloc(sizeof(double)*sizeplane);
            double* dataUpperPlaneVs = (double *)malloc(sizeof(double)*sizeplane);
            double* dataUpperPlaneRho = (double *)malloc(sizeof(double)*sizeplane);
            int i;
            int j=0;
            int k=0;
            for (i=LowerPlanePosVector_i; i<LowerPlanePosVector_j+1; i++)
            {
                dataLowerPlaneVp[j]=dataPlaneOutput[i][4];
                dataLowerPlaneVs[j]=dataPlaneOutput[i][5];
                dataLowerPlaneRho[j]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                j=j+1;
            }
            for (i=UpperPlanePosVector_i; i<UpperPlanePosVector_j+1; i++)
            {
                dataUpperPlaneVp[k]=dataPlaneOutput[i][4];
                dataUpperPlaneVs[k]=dataPlaneOutput[i][5];
                dataUpperPlaneRho[k]=dataPlaneOutput[i][6];
                //printf("%lf %lf %lf \n", dataPlaneVp[j], dataPlaneVs[j], dataPlaneRho[j]);
                k=k+1;
            }
            CVMResult[0]=interp3(NPlanesBogota,sizeplane,dataLowerPlaneVp,dataUpperPlaneVp,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx,depthVector);
            CVMResult[1]=interp3(NPlanesBogota,sizeplane,dataLowerPlaneVs,dataUpperPlaneVs,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx,depthVector);
            CVMResult[2]=interp3(NPlanesBogota,sizeplane,dataLowerPlaneRho,dataUpperPlaneRho,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx,depthVector);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]*1000);
            free(dataLowerPlaneVp);
            dataLowerPlaneVp=NULL;
            free(dataLowerPlaneVs);
            dataLowerPlaneVs=NULL;
            free(dataLowerPlaneRho);
            dataLowerPlaneRho=NULL;
            free(dataUpperPlaneVp);
            dataUpperPlaneVp=NULL;
            free(dataUpperPlaneVs);
            dataUpperPlaneVs=NULL;
            free(dataUpperPlaneRho);
            dataUpperPlaneRho=NULL;
        }
        int i;
        for (i=0; i<nx;i++)
        {
            free(DiffXcoor[i]);
        }
        DiffXcoor=NULL;
        free(DiffXcoor);
        for (i=0; i<ny;i++)
        {
            free(DiffYcoor[i]);
        }
        free(DiffYcoor);
        DiffYcoor=NULL;
    }
    payload->Vp=CVMResult[0];
    payload->Vs=CVMResult[1];
    payload->rho=CVMResult[2]*1000; // density in kg/m3
    //printf("%.11lf %.11lf %lf %lf %lf %lf\n", coordinatesVector[0], coordinatesVector[1], coordinatesVector[2],CVMResult[0],CVMResult[1],CVMResult[2]*1000);
    //printf("%lf %lf %lf %.11lf %.11lf %lf %lf %lf %lf\n", XcoorPoint, XcoorPoint, DepthPoint, coordinatesVector[0], coordinatesVector[1], coordinatesVector[2],payload->Vp,payload->Vs,payload->rho);
    // Dealocate memory assigned to the structures
    free(xcoorvector);
    xcoorvector=NULL;
    free(ycoorvector);
    ycoorvector=NULL;
    free(coordinatesVector);
    coordinatesVector=NULL;
    if (CVMResult[0] == 0 || CVMResult[1] == 0 || CVMResult[2] == 0)
    {
        printf("%s\n", "CVM ERROR");
        free(CVMResult);
        CVMResult=NULL;
        return -1; //error
    }
    free(CVMResult);
    CVMResult=NULL;
    //!!!!!free los dos arreglos nuevos luego del enmallado en hercules!!!!!
    return 0;
}
