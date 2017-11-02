//
//  CVMBogotaLibraryHercules.c
//  CVMBogota
//
//  Created by Andrea camila Riaño escandon  on 10/30/17.
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
    
    // Latitudes of Bogota region corners
    return bilinear(x,y,
                    0, 0, 130402.370855, 101000.008112,
                    4.460303, 3.799435,
                    5.216928, 4.555367);
}

double get_lon(double x, double y) {
    
    // Longitudes of ShakeOut region corners
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
    for (i = 0; i<NData; i++)
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
    int i;
    for ( i=0; i<nData; i++)
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
    for ( i=0; i<nData; i++)
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
    for ( i=0; i<nData; i++)
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
double interp2(int Nsizeplane, double VectorData[Nsizeplane],double* xcoorvector,double* ycoorvector,double x,double y, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax)
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
double interp3(int ndepths ,int Nsizeplane, double VectorDataLower[Nsizeplane],double VectorDataUpper[Nsizeplane],double* xcoorvector,double* ycoorvector,double x,double y, double z, int IndexClosestNeg, int IndexClosestPos, int IndexClosestNegX, int IndexClosestPosX,int IndexClosestNegY,int IndexClosestPosY, int IndexZeroX,int IndexZeroY, int method, int NDatax, double depthVector[ndepths])
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
int CVMBogota(double XcoorPoint,double YcoorPoint,double DepthPoint,cvmpayload_t* payload)
{
    // A polygon inside the CVM region
    double distXcornerc11 = 0;     //if = 0 it means that the simulation area starts in the x origin of the CVM region
    double distYcornerc11 = 30000; //if = 0 it means that the simulation area starts in the y origin of the CVM region
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

    for ( i=1; i<nx;i++)
    {
        xcoorvector[i]=xcoorvector[i-1]+GridSpacing;
        //printf("%lf\n", xcoorvector[i]);  // debug chekpoint
        
    }
    for ( i=1; i<ny;i++)
    {
        ycoorvector[i]=ycoorvector[i-1]+GridSpacing;
        //printf("%lf\n", ycoorvector[i]);  // debug chekpoint
    }
    
    // The following lines of code read a text file containing the path of each plane --------------------------------------------------------------------------------------------------------
    FILE *filePlanes = fopen("/u/sciteam/rianoesc/bogotadatabase/FilesPlanesName.txt","r");
    
    int entryCount;
    fscanf(filePlanes, "%i", &entryCount);
    Entry *entries = malloc(sizeof(Entry)*entryCount); // for 82 entries structs
    
    for ( i=0; i<entryCount;i++)
    {
        int nameLen;
        fscanf(filePlanes,"%i",&nameLen);
        entries[i].name = malloc (sizeof(char)*(nameLen+1));
        fscanf(filePlanes,"%s %i %lf", entries[i].name, &entries[i].ID, &entries[i].Depth);
        //printf("%i. %s %i %lf\n",i+1,entries[i].name,entries[i].ID, entries[i].Depth);  // debug chekpoint
    }
    
    fclose(filePlanes); // Clean up
    
    // load polygon coordinates in the local system of Hercules --------------------------------------------------------------------------------------------------------
    FILE *fileCoordinates = fopen("/u/sciteam/rianoesc/bogotadatabase/CoordinatesStudyArea.txt","r");
    Coordinates *coordinates = malloc(sizeof(Coordinates)*sizeplane); // for sizeplane entries structs
    
    for ( i=0; i<sizeplane;i++)
    {
        fscanf(fileCoordinates,"%lf %lf", &coordinates[i].xcoor, &coordinates[i].ycoor);
        //printf("%lf %lf\n",coordinates[i].xcoor,coordinates[i].ycoor);  // debug chekpoint
    }
    
    fclose(fileCoordinates); // Clean up
    
    
    // Find the planes or plane that are/is needed to interpolate depending on the depth given as input --------------------------------------------------------------------------------------------------------
    /* 1. Find the difference vector between the depth of the point and available depths */
    double depthVector[entryCount];
    for ( i=0; i<entryCount;i++)
    {
        depthVector[i]=entries[i].Depth;
        //printf("%lf \n", depthVector[i]); //debug point to check depthvector
    }
    double** diff;
    diff=DiffVector(entryCount,depthVector,DepthPoint);
    
    /* 2. find if the min dif == max dif == 0 or if min dif =~ max dif */
    int IndexZero;
    int IndexClosestNeg;
    int IndexClosestPos;
    IndexZero = DiffEqualZero(entryCount,diff); //find index for diff=0, if result = -1 there's no diff=0
    IndexClosestNeg = ClosestNeg(entryCount,diff);
    IndexClosestPos = ClosestPos(entryCount,diff);
    
    
    /* 3. Define if I need to perform a 2D interp or a 3D interp. If IndexZero=~0 2D interp, 3D interp otherwise. */
    //------------------------------------------------------------------------------------------If IndexZero == -1 means that there is not a plane with the input Depth
    // 2D INTERPD
    if (IndexZero != -1) {
        // Loading plane data  - Only one plane needed for 2D interp ---------------------------------------------------------------------------------------------
        
        int nPlanes = 0; //this is always 0
        CVMPlane *planeData = malloc(sizeof(CVMPlane)*1); // for 1 plane structs
        //printf("%lu",sizeof(CVMPlane)*entryCount); // 8 bytes * 7 properties * NPlanes planes * 120701 data/property --> debug chekpoint
        
        FILE *file = fopen(entries[IndexZero].name,"r");
        //printf("%s\n",entries[IndexZero].name);
        int j;
        for ( j=0; j<sizeplane;j++)
        {
            fscanf(file,"%lf %lf %lf %lf %lf %lf %lf", &planeData[nPlanes].Xcoor[j], &planeData[nPlanes].Ycoor[j], &planeData[nPlanes].Elevation[j], &planeData[nPlanes].Depth[j], &planeData[nPlanes].Vp[j], &planeData[nPlanes].Vs[j], &planeData[nPlanes].Ro[j]);
            //printf("%lf %lf %lf %lf %lf %lf %lf", planeData[IndexZero].Xcoor[j], planeData[IndexZero].Ycoor[j], planeData[IndexZero].Elevation[j], planeData[IndexZero].Depth[j], planeData[IndexZero].Vp[j], planeData[IndexZero].Vs[j], planeData[IndexZero].Ro[j]);
            //printf("\n");
        }
        
        // Clean up
        fclose(file);
        
        
        // 2D INTERPOLATION STARTS HERE --------------------------------------------------------------------------------------------------
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
            int posData = IndexZeroX + (nx*IndexZeroY);
            CVMResult[0]=planeData[nPlanes].Vp[posData];
            CVMResult[1]=planeData[nPlanes].Vs[posData];
            CVMResult[2]=planeData[nPlanes].Ro[posData];
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]);
            
        }
        else if (IndexZeroX == -1 && IndexZeroY == -1) // Bilinear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            CVMResult[0]=interp2(sizeplane,planeData[nPlanes].Vp,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx);
            CVMResult[1]=interp2(sizeplane,planeData[nPlanes].Vs,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx);
            CVMResult[2]=interp2(sizeplane,planeData[nPlanes].Ro,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]);
        }
        else if (IndexZeroX != -1 && IndexZeroY == -1) // linear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            CVMResult[0]=interp2(sizeplane,planeData[nPlanes].Vp,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[1]=interp2(sizeplane,planeData[nPlanes].Vs,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[2]=interp2(sizeplane,planeData[nPlanes].Ro,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]);
        }
        else if (IndexZeroX == -1 && IndexZeroY != -1) // linear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            CVMResult[0]=interp2(sizeplane,planeData[nPlanes].Vp,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[1]=interp2(sizeplane,planeData[nPlanes].Vs,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            CVMResult[2]=interp2(sizeplane,planeData[nPlanes].Ro,xcoorvector,ycoorvector,x,y,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]);
        }
        free(planeData); ///////////////////////////
    }
    else
    {
        int nPlanes = 0; //this is always 0
        CVMPlane *planeDataLower = malloc(sizeof(CVMPlane)*1); // for 1 plane structs
        CVMPlane *planeDataUpper = malloc(sizeof(CVMPlane)*1); // for 1 plane structs
        //printf("%lu",sizeof(CVMPlane)*entryCount); // 8 bytes * 7 properties * NPlanes planes * 120701 data/property --> debug chekpoint
        
        FILE *fileLowerPlane = fopen(entries[IndexClosestPos].name,"r");
        FILE *fileUpperPlane = fopen(entries[IndexClosestNeg].name,"r");
        //printf("%s\n",entries[IndexClosestPos].name);
        //printf("%s\n",entries[IndexClosestNeg].name);
        int j;
        for ( j=0; j<sizeplane;j++ )
        {
            fscanf(fileLowerPlane,"%lf %lf %lf %lf %lf %lf %lf", &planeDataLower[nPlanes].Xcoor[j], &planeDataLower[nPlanes].Ycoor[j], &planeDataLower[nPlanes].Elevation[j], &planeDataLower[nPlanes].Depth[j], &planeDataLower[nPlanes].Vp[j], &planeDataLower[nPlanes].Vs[j], &planeDataLower[nPlanes].Ro[j]);
            
            fscanf(fileUpperPlane,"%lf %lf %lf %lf %lf %lf %lf", &planeDataUpper[nPlanes].Xcoor[j], &planeDataUpper[nPlanes].Ycoor[j], &planeDataUpper[nPlanes].Elevation[j], &planeDataUpper[nPlanes].Depth[j], &planeDataUpper[nPlanes].Vp[j], &planeDataUpper[nPlanes].Vs[j], &planeDataUpper[nPlanes].Ro[j]);
            
            //printf("%lf %lf %lf %lf %lf %lf %lf", planeData[IndexZero].Xcoor[j], planeData[IndexZero].Ycoor[j], planeData[IndexZero].Elevation[j], planeData[IndexZero].Depth[j], planeData[IndexZero].Vp[j], planeData[IndexZero].Vs[j], planeData[IndexZero].Ro[j]);
            //printf("\n");
        }
        // Clean up
        fclose(fileLowerPlane);
        fclose(fileUpperPlane);
        
        
        // 3D INTERPOLATION STARTS HERE --------------------------------------------------------------------------------------------------
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
            CVMResult[0]=interp3(entryCount,sizeplane,planeDataLower[nPlanes].Vp,planeDataUpper[nPlanes].Vp,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx,depthVector);
            CVMResult[1]=interp3(entryCount,sizeplane,planeDataLower[nPlanes].Vs,planeDataUpper[nPlanes].Vs,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx,depthVector);
            CVMResult[2]=interp3(entryCount,sizeplane,planeDataLower[nPlanes].Ro,planeDataUpper[nPlanes].Ro,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,1,nx,depthVector);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n",CVMResult[0], CVMResult[1], CVMResult[2]);
        }
        else //if (IndexZeroX != -1 && IndexZeroY != -1) // linear interp
        {
            double x=XcoorPoint+distXcornerc11;
            double y=YcoorPoint+distYcornerc11;
            double z=DepthPoint;
            CVMResult[0]=interp3(entryCount,sizeplane,planeDataLower[nPlanes].Vp,planeDataUpper[nPlanes].Vp,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx,depthVector);
            CVMResult[1]=interp3(entryCount,sizeplane,planeDataLower[nPlanes].Vs,planeDataUpper[nPlanes].Vs,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx,depthVector);
            CVMResult[2]=interp3(entryCount,sizeplane,planeDataLower[nPlanes].Ro,planeDataUpper[nPlanes].Ro,xcoorvector,ycoorvector,x,y,z,IndexClosestNeg,IndexClosestPos,IndexClosestNegX,IndexClosestPosX,IndexClosestNegY,IndexClosestPosY,IndexZeroX,IndexZeroY,2,nx,depthVector);
            //printf("Vp, %lf - Vs, %lf - Density, %lf \n", CVMResult[0], CVMResult[1], CVMResult[2]);
        }
        free(planeDataLower);
        free(planeDataUpper);
    }
    
    // Dealocate memory assigned to the structure entries PlanesData and entries.name
    int ii;
    for ( ii=0; ii<entryCount;ii++)
    {
        free(entries[ii].name);
    }
    free(entries);
    free(diff);
    free(coordinatesVector);
    //printf("\n");
    
    // Define coordinates vector for interpolation --------------------------------------------------------------------------------------------------------
    payload->Vp=CVMResult[0];
    payload->Vs=CVMResult[1];
    payload->rho=CVMResult[2]*1000; // density in kg/m3
    //printf("%.11lf %.11lf %lf %lf %lf %lf\n", coordinatesVector[0], coordinatesVector[1], coordinatesVector[2],CVMResult[0],CVMResult[1],CVMResult[2]);
    //printf("%lf %lf %lf\n", payload->Vp,payload->Vs,payload->Ro);
    free(CVMResult);
    return 0;
}
