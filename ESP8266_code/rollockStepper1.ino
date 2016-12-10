#include <Stepper.h>

Stepper motor1(200,D1,D2,D3,D4); //Change to motor1,2,3,4
int motorData[2];
int motorSpeed1=0;
int motorStep1=0;
int motorStep2=0;
int motorStep3=0;
int t=0;
int s=0;
int k=0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  //wdt_enable(WDTO_8S);
}

void motorSpeed(){
  motor1.setSpeed(motorSpeed1);
}

void motorStep(){
  wdt_disable();
  motor1.step(motorStep3);
  wdt_enable(WDTO_8S);
  //Serial.print("A");
}

void loop() {
  if (Serial.available() > 0 && t==0)
  {
    s=Serial.read();
    k=pow(-1,s);
    t=1;
  }
  if (Serial.available() > 0 && t==1)//Send data only when it receives data
  {
  motorStep1=Serial.read();
  t=2;
  }
  if (Serial.available() > 0 && t==2)
  {
  motorStep2=Serial.read();
  t=3;  
  }
  if (Serial.available() > 0 && t==3)
  {
  motorSpeed1=Serial.read();
  motorStep3=(motorStep1*256 + motorStep2)*(k);
  //Serial.println(motorStep3);
  //Serial.println(motorSpeed1);
  motorSpeed();
  motorStep();
  t=0;
  }
} 
