#include "mbed.h"
#include <vector>
#include <algorithm> 
#include <iostream>
#include <ctime>
#include "Rollock.h"
using namespace std;
using std::vector;
using std::random_shuffle;

Serial pc(USBTX, USBRX);
Serial uart2(PD_5,PD_6);  //motor 0
Serial uart3(PB_10,PB_11);
Serial uart4(PA_0,PC_11);  //paint jig
Serial uart5(PC_12,PD_2);   //motor 1
Serial uart6(PG_14,PC_7); //motor 2
Serial uart7(PE_8,PE_7);    //motor 3
InterruptIn mybutton(USER_BUTTON);

int i = 0;
int txFlag = 0;
int size = 0;
float pointData;
int pointDataByte;
int numDataPoints = 0;
int sendData = 0;
int movementNum = 0;
int uart3rx = 0;
int uart2rx = 0;
int uart5rx = 0;
int uart6rx = 0;
int uart7rx = 0;

double currentPosition[] = {0,0,0};
vector<int> wayPointOrder;
vector<int> emotions;
int steps[] = {0,0,0,0};
int speeds[] = {1,1,1,1};
double nextWaypoint[3];
int nextEmotion = 0;

typedef enum _rollockState
{
    IDLE,
    MOVE,
    FINISHED,
    
}rollockState;

rollockState state;
int myrandom (int i) { return std::rand()%i;}
 
int main() 
{
    printf("Press any key to begin: \r\n");
    srand(time(0));
    
    CreateGrid();
    SetToHomePosition();
    state = IDLE;
    
    mybutton.fall(&pressed);
//    uart3.attach(&Uart3Ack,Serial::RxIrq);
//    uart2.attach(&Uart2Ack,Serial::RxIrq);
//    uart5.attach(&Uart5Ack,Serial::RxIrq);
//    uart6.attach(&Uart6Ack,Serial::RxIrq);
//    uart7.attach(&Uart7Ack,Serial::RxIrq);
    pc.baud(9600);
    
 //   sendFakeParameters(1);
//    while(1)
//    {
//        if(uart2.readable())
//        {   
//            pc.printf("ack received!: ");
//            pc.printf("%d\r\n", uart2.getc());
//            break;
//        }    
//    }
//    sendFakeParameters(2);
//    while(1)
//    {
//        uart3.printf("Working... got here\r\n");
//        wait(1);
//    };
   
    //populate the order array
    for (int i=0; i<(NUM_WAYPOINTS); ++i) wayPointOrder.push_back(i);
    
    for (int i=0; i<(ANGER); ++i) emotions.push_back(0);
    
    for (int i=0; i<(FEAR); ++i) emotions.push_back(1);
    
    for (int i=0; i<(JOY); ++i) emotions.push_back(2);
     
    for (int i=0; i<(SADNESS); ++i) emotions.push_back(3);
    
    printf("Now In the array:\r\n");
    for (int i=0; i<NUM_WAYPOINTS; ++i) printf("%d ",wayPointOrder[i]);
    printf("\r\nEmotions:\r\n");
    for (int i=0; i<NUM_WAYPOINTS; ++i) printf("%d ",emotions[i]);
    printf("\r\nDone\r\n");
    
//    random_shuffle (wayPointOrder.begin(), wayPointOrder.end(), myrandom);

    random_shuffle (emotions.begin(), emotions.end(), myrandom);
    
    printf("\r\nEmotions randomised:\r\n");
    for (int i=0; i<NUM_WAYPOINTS; ++i) printf("%d ",emotions[i]);
    printf("\r\nDone\r\n");
    
    wayPointOrder[0] = 12;
    
    wayPointOrder[1] = 10;
    wayPointOrder[2] = 2;
    wayPointOrder[3] = 14;
    wayPointOrder[4] = 22;
    
    wayPointOrder[5] = 10;
    wayPointOrder[6] = 2;
    wayPointOrder[7] = 14;
    wayPointOrder[8] = 22;
    
    wayPointOrder[9] = 10;
    wayPointOrder[10] = 2;
    wayPointOrder[11] = 14;
    wayPointOrder[12] = 22;
    
    wayPointOrder[13] = 24;

    
    printf("Now In the array:\r\n");
    for (int i=0; i<NUM_WAYPOINTS; ++i) printf("%d ",wayPointOrder[i]);
    printf("\r\nDone\r\n");
    

    NextWayPoint(wayPointOrder[0],&nextWaypoint[0]);
    printf("nwp x = %f:\r\n", nextWaypoint[0]);
    printf("nwp y = %f:\r\n", nextWaypoint[1]);
    printf("nwp z = %f:\r\n", nextWaypoint[2]);
    
    printf("\r\nSetup Complete!\r\n");
    while(1) 
    {
        switch(state)
        {
            case IDLE:
                if(uart2.readable())
                {   
                    pc.printf("ack received!: ");
                    pc.printf("%d\r\n", uart2.getc());
                    
                    break;
                } 
                break;
                
            case MOVE:
                if(movementNum <= NUM_WAYPOINTS)
                {
                    nextEmotion = emotions[movementNum];    
                    NextWayPoint(wayPointOrder[movementNum],&nextWaypoint[0]);
                    CalculateParameters(&currentPosition[0],&nextWaypoint[0], &steps[0], &speeds[0]);
                    sendParameters();
                    printf("MovementNum %d complete:\r\n", movementNum);
                    movementNum++;
                }
                state = IDLE;
                if(movementNum >= NUM_WAYPOINTS)
                {
                    state = FINISHED;
                }
                break;
                
            case FINISHED:
                printf("Run complete:\r\n");
                wait(10);
                break;   
        }    
    }
}

void sendFakeParameters(int symbol)
{
    //1 = negative 2 = positive
    //uart3.printf("Sign: %d \r\n", symbol);
    uart2.putc(symbol);
    uart5.putc(symbol);
    uart6.putc(symbol);
    uart7.putc(symbol);
    wait(1);
    
    uart3.printf("Motor Steps: %d\r\n", 0xC8);
    uart2.putc(0x00);
    uart5.putc(0x00);
    uart6.putc(0x00);
    uart7.putc(0x00);
    wait(1);
    
    //send LSB
    uart2.putc(0xC8);
    uart5.putc(0xC8);
    uart6.putc(0xC8);
    uart7.putc(0xC8);
    wait(1);
    
    uart3.printf("Motor 0 speed: %d\r\n", 40);
    uart2.putc(40);
    uart3.printf("Motor 1 speeds: %d\r\n", 40);
    uart5.putc(40);
    uart3.printf("Motor 2 speeds: %d\r\n", 40);
    uart6.putc(40);
    uart3.printf("Motor 3 speeds: %d\r\n", 40);
    uart7.putc(40);
}

void sendParameters()
{
    int index = 0;
    int sign[] = {0,0,0,0};
    int stepsHigh[] = {0,0,0,0};
    int stepsLow[] = {0,0,0,0};
    
    if(movementNum > 0)
    {
        //stop previous paint flow
        uart3.printf("Stop paint jig(motor number): %d close to: %d \r\n",emotions[movementNum-1],155);
        uart4.printf("155");
        wait(0.5);
        uart4.printf("%d",emotions[movementNum-1]);
    }
        
    
    uart3.printf("steps before: %d, %d, %d, %d \r\n",steps[0],steps[1],steps[2],steps[3]); 
    for(index = 0; index<4; index++)
    {
        if(steps[index] < 0)
        {
            sign[index] = 2;   //if we are shortening the cable then send a 2 (its was implemented the opposite way it should be)
            steps[index] *= -1;
        }else
        {
            sign[index] = 1;    //if we are lengthening the cable then send a 1
        }
        stepsHigh[index] = ((steps[index] & 0xFF00) >> 8);
        stepsLow[index] = (steps[index] & 0xFF);
        
    }
    uart3.printf("signs: %d, %d, %d, %d \r\n",sign[0],sign[1],sign[2],sign[3]); 
    //send the signs
    uart2.putc(sign[0]);
    uart5.putc(sign[1]);
    uart6.putc(sign[2]);
    uart7.putc(sign[3]);
    wait(0.5);
    
    uart3.printf("steps after: %d, %d, %d, %d \r\n",steps[0],steps[1],steps[2],steps[3]);
    uart3.printf("steps higher: %x, %x, %x, %x \r\n",stepsHigh[0],stepsHigh[1],stepsHigh[2],stepsHigh[3]);
    uart3.printf("steps lower: %x, %x, %x, %x \r\n",stepsLow[0],stepsLow[1],stepsLow[2],stepsLow[3]);
    
    //send the steps MSB
    uart2.putc(stepsHigh[0]);
    uart5.putc(stepsHigh[1]);
    uart6.putc(stepsHigh[2]);
    uart7.putc(stepsHigh[3]);
    wait(0.5);
    
    //send the steps LSB
    uart2.putc(stepsLow[0]);
    uart5.putc(stepsLow[1]);
    uart6.putc(stepsLow[2]);
    uart7.putc(stepsLow[3]);
    wait(0.5);
    
    uart3.printf("Next emotion (motor number): %d open to: %d \r\n",nextEmotion,120);
    uart4.printf("110");
    wait(0.5);
    uart4.printf("%d",nextEmotion);
    
   
    uart3.printf("speeds: %d, %d, %d, %d \r\n",speeds[0],speeds[1],speeds[2],speeds[3]); 
    
    //send speed
    uart2.putc(speeds[0]);
    uart5.putc(speeds[1]);
    uart6.putc(speeds[2]);
    uart7.putc(speeds[3]);
    
    uart3.printf("DONE! \r\n");

}

void pressed()
{
    state = MOVE;
}

void Uart3Ack()
{
    printf("Uart3 acked: ");
    //printf("%d\r\n", uart3.getc());
    uart3rx = 1;
}

void Uart2Ack()
{
    printf("Uart2 acked: ");
    if(uart2.readable())
    {
        printf("%d\r\n", uart2.getc());
    }
    uart2rx = 1; 
}

void Uart5Ack()
{
    printf("Uart5 acked: ");
    //printf("%d\r\n", uart5.getc());
    uart5rx = 1; 
}

void Uart6Ack()
{
    printf("Uart2 acked: ");
    //printf("%d\r\n", uart5.getc());
    uart6rx = 1; 
}

void Uart7Ack()
{
    printf("Uart2 acked: ");
    //printf("%d\r\n", uart7.getc()); 
    uart7rx = 1;
}