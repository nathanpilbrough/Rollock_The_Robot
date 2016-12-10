#include "mbed.h"
#include "math.h"
#include <algorithm> 
#include "Rollock.h"

double motorxPos[] = {motorx0,motorx1,motorx2,motorx3};
double motoryPos[] = {motory0,motory1,motory2,motory3};
double motorzPos[] = {motorz0,motorz1,motorz2,motorz3};

double _currentLength[] = {0,0,0,0};
double _newLength[] = {0,0,0,0};
double _changeInLength[] = {0,0,0,0};
double _changeInLengthAbs[] = {0,0,0,0};

void CalculateLength(double newPosition[], double lengthArray[]);
void UpdateState(double currentPosition[], double newPosition[],double currentLength[], double newLength[]);

void SetToHomePosition()
{
    double homeCoords[] = {405,270,180};
    CalculateLength(&homeCoords[0], &_currentLength[0]);
    printf("Home Lengths:\r\n");
    printf("%f\r\n", _currentLength[0]);
    printf("%f\r\n", _currentLength[1]);
    printf("%f\r\n", _currentLength[2]);
    printf("%f\r\n", _currentLength[3]);
}

void CalculateLength(double newPosition[], double lengthArray[])
{   
    int index = 0;
    double xOffset[] = {motor0XOffset,motor1XOffset,motor2XOffset,motor3XOffset};
    double yOffset[] = {motor0YOffset,motor1YOffset,motor2YOffset,motor3YOffset};
    for(index = 0; index<4; index++)
    {
        lengthArray[index] = sqrt(pow(((motorxPos[index]) - (newPosition[0] + xOffset[index])),2) 
                                + pow(((motoryPos[index]) - (newPosition[1] + yOffset[index])),2) 
                                + pow((motorzPos[index] - newPosition[2]),2));
        printf("New length %d = %f\r\n",index, lengthArray[index]);
    }
}

void ChangeInLength(double currentLength[], double newLength[], double changeInLengthAbs[], double changeInLength[])
{   
    int index = 0;
    //printf("Change in length:\r\n");
    for(index = 0; index<4; index++)
    {
        changeInLengthAbs[index] = fabs(newLength[index]- currentLength[index]);
        changeInLength[index] = newLength[index]- currentLength[index]; 
        printf("Change in length %d = %f\r\n",index, changeInLength[index]);
    }
    
}

void CalculateSpeed(double changeInLengthAbs[], int speeds[], int maxSpeed)
{
    //not sure why this is 4 but it only works with 4, i thought it should be 3 but NO just No
    double *maxlength = std::max_element(changeInLengthAbs,changeInLengthAbs+4); 
    int index = 0;
    for(index = 0; index<4; index++)
    {
        speeds[index] = (changeInLengthAbs[index]/(*maxlength))*maxSpeed;
    }
}

void CalculateSteps(double changeInLength[], int steps[])
{
    int index = 0;
    double lengthToStepRatio = 0.295;
    //printf("Steps:\r\n");
    for(index = 0; index<4; index++)
    {
        steps[index] = changeInLength[index]/lengthToStepRatio;
        //printf("%d\r\n", steps[index]);
    }
}

void UpdateState(double currentPosition[], double newPosition[],double currentLength[], double newLength[])
{
    int index = 0;
    printf("New position:\r\n");
    for(index = 0; index<3; index++)
    {
        currentPosition[index] = newPosition[index];
        printf("%f\r\n", newPosition[index]);   
    }
    //printf("New lengths:\r\n");
    for(index = 0; index<4; index++)
    {
        currentLength[index] = newLength[index];   
        //printf("%f\r\n", newLength[index]);
    }
}
    
void CalculateParameters(double currentPosition[], double newPosition[], int steps[], int speeds[])
{
    CalculateLength(&newPosition[0],&_newLength[0]);
    ChangeInLength(&_currentLength[0],&_newLength[0], &_changeInLengthAbs[0], &_changeInLength[0]);
    CalculateSpeed(&_changeInLengthAbs[0],&speeds[0], MAX_SPEED);
    CalculateSteps(&_changeInLength[0],&steps[0]);
    UpdateState(&currentPosition[0],&newPosition[0],&_currentLength[0],&_newLength[0]);
}

