#include "mbed.h"
#include <vector>
#include <algorithm> 
#include <iostream>
#include "Rollock.h"

vector<vector<double> > wayPoints;

void CreateGrid()
{
    int x = X_START_POINT ;
    int y = Y_START_POINT;
    wayPoints.resize(NUM_WAYPOINTS);
    for (int i = 0; i < NUM_WAYPOINTS; ++i)
    {
        wayPoints[i].resize(ARRAY_WIDTH);
    }
    
    for (int j = 0; j <  NUM_WAYPOINTS ; ++j)
    {
            wayPoints[j][0] = x;
            wayPoints[j][1] = y;
            wayPoints[j][2] = 350;
            
            printf("wayPoint x%d = %f ",j ,wayPoints[j][0]);
            printf("wayPoint y%d = %f ",j ,wayPoints[j][1]);
            printf("wayPoint z%d = %f \r\n",j ,wayPoints[j][2]);
            
            x = x + X_INCREMENT;
            if(x >= ((GRID_WIDTH*X_INCREMENT)+ X_START_POINT))
            {
                x = X_START_POINT;
                y = y + Y_INCREMENT;    
            }        
    }
    wayPoints[24][0] = 405;
    wayPoints[24][1] = 270;
    wayPoints[24][2] = 100;   
    printf("wayPoint x%d = %f ",24 ,wayPoints[24][0]);
    printf("wayPoint y%d = %f ",24 ,wayPoints[24][1]);
    printf("wayPoint z%d = %f \r\n",24 ,wayPoints[24][2]);
}

void NextWayPoint(int wayPointNum, double nextWayPoint[])
{   
    double xrand = (((double)myrandom(200))/100) - 1;
    double yrand = (((double)myrandom(200))/100) - 1;
    double zrand = -(myrandom(LOWEST_Z_POINT));
    
    printf("xrand = %f\r\n", xrand);   
    printf("yrand = %f\r\n", yrand);   
    printf("zrand = %f\r\n", zrand);   
      
    nextWayPoint[0] = wayPoints[wayPointNum][0];// + xrand;    
    nextWayPoint[1] = wayPoints[wayPointNum][1];// + yrand;
    nextWayPoint[2] = wayPoints[wayPointNum][2];// + zrand;
    
    
}