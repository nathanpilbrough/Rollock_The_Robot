//Proogram #defines
//Grid config
#define GRID_HEIGHT 5
#define GRID_WIDTH 5

#define NUM_WAYPOINTS (GRID_HEIGHT*GRID_WIDTH)
#define ARRAY_HEIGHT (GRID_HEIGHT*GRID_WIDTH)
#define ARRAY_WIDTH 3

#define X_START_POINT 205
#define Y_START_POINT 70

#define X_INCREMENT 100
#define Y_INCREMENT 100

//Emotions
#define ANGER   6
#define FEAR    6
#define JOY     6
#define SADNESS 7

//Motor Positions
#define motorx0 0
#define motory0 0
#define motorz0 520

#define motorx1 810
#define motory1 0
#define motorz1 520

#define motorx2 0
#define motory2 540
#define motorz2 520

#define motorx3 810
#define motory3 540
#define motorz3 520

//Motor Offset
//#define motor0XOffset 0
//#define motor1XOffset 0
//#define motor2XOffset 0
//#define motor3XOffset 0
//
//#define motor0YOffset 0
//#define motor1YOffset 0
//#define motor2YOffset 0
//#define motor3YOffset 0

#define motor0XOffset -57.5
#define motor1XOffset 57.5
#define motor2XOffset -57.5
#define motor3XOffset 57.5

#define motor0YOffset -39
#define motor1YOffset -39
#define motor2YOffset 39
#define motor3YOffset 39

//other parameters 
#define MAX_SPEED 300
#define LOWEST_Z_POINT 520

//Function Declarations
int myrandom (int i);
void pressed();
void Uart3Ack();
void Uart2Ack();
void Uart5Ack();
void Uart6Ack();
void Uart7Ack();
void SetToHomePosition();
void CalculateLength(double newPosition[], double lengthArray[]);
void ChangeInLength(double currentLength[], double newLength[], double changeInLengthAbs[], double changeInLength[]);
void CalculateSpeed(double changeInLengthAbs[], int speeds[], int maxSpeed);
void CalculateParameters(double currentPosition[], double newPosition[], int steps[], int speeds[]);
void CalculateSteps(double changeInLength[], int steps[]);
void CreateGrid();
void NextWayPoint(int wayPointNum, double nextWayPoint[]);
void sendFakeParameters(int symbol);
void sendParameters();