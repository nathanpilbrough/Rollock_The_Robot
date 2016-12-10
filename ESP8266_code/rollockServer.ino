#include <ESP8266WiFi.h>
#include <Servo.h>

char*       TKDssid;              // SERVER WIFI NAME
char*       TKDpassword;          // SERVER PASSWORD
int t;
int ws;
int wp;
String valuemotor;
Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

#define MAXSC 6                 //Maximum number of client

WiFiServer  TKDServer(9001);      // THE SERVER AND THE PORT NUMBER
WiFiClient  TKDClient[MAXSC];     // THE SERVER CLIENTS


void setup()
{
  // Setting The Serial Port
  Serial.begin(9600);           // Computer Communication
    
      // Setting The Mode Of Pins
  pinMode(LED_BUILTIN, OUTPUT);          // Indicator For Client #1 Connectivity
  //pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    
  // Print Message Of I/O Setting Progress
  //Serial.println();
  //Serial.println("I/O Pins Modes Set .... Done");

  // Setting Up A Wifi Access Point
  SetWifi("TAKEONE", "");

  servo0.attach(D0);
  servo0.write(155);
  servo1.attach(D1);
  servo1.write(155);
  servo2.attach(D2);
  servo2.write(155);
  servo3.attach(D3);
  servo3.write(155);
}


void loop()
{
  IsClients();
}

void SetWifi(char* Name, char* Password)
{
  WiFi.disconnect(); //Stop Any Previous WIFI

  //Setting The Wifi Mode
  WiFi.mode(WIFI_AP_STA);
  //Serial.println("Wifi Mode: AccessPoint Station");

  //Setting the AccessPoint Name & Password
  TKDssid=Name;
  TKDpassword=Password;

  //Starting the access point
  WiFi.softAP(TKDssid, TKDpassword);
  //Serial.println("Wifi < " + String(TKDssid) + " > ... Started");

  //Wait for few moments
  delay(1000);

  IPAddress IP = WiFi.softAPIP(); //Getting Server IP

  //Serial.print("AccessPoint IP: "); //Printing the server IP address
  //Serial.println(IP);

  TKDServer.begin(); //Starting Server
  //Serial.println("Server Started");
}

  void IsClients()
  {
    if(TKDServer.hasClient())
    {
      WiFiClient TKDClient = TKDServer.available();
      digitalWrite(LED_BUILTIN, LOW); //LED to indicate conection
      while(1)
      {
        // If Clients Are Connected 
        
        if(TKDClient.available() && t==0)
        {
          // Here We Read The Message
          valuemotor = TKDClient.readString();
          Serial.println(valuemotor);
          wp=valuemotor.toInt();
          ws=wp;
          wp=wp/10;
          ws=ws%10;
          t=1;
          Serial.println(ws);
          Serial.println(wp);
          
          //SERVO 0
          if (ws==0)
          {
           servo0.write(wp);
           TKDClient.flush();
           t=0;
           }
           
           //SERVO 1
           if (ws==1)
           {
            servo1.write(wp);
            TKDClient.flush();
            t=0;
           }
           
           //SERVO 2     
           if (ws==2)
          {
           servo2.write(wp);
           TKDClient.flush();
           t=0;
           }
           
           //SERVO 3
           if (ws==3)
           {
            servo3.write(wp);
            TKDClient.flush();
            t=0;
           } 

            TKDClient.flush();
        }
        //---------------------------------------------------------------
        // If Clients Are Disconnected
        //---------------------------------------------------------------
        
        if(!TKDClient || !TKDClient.connected())
        {
          // Here We Turn Off The LED To Indicated The Its Disconnectted
          digitalWrite(LED_BUILTIN, HIGH);
          // Here We Jump Out Of The While Loop
          break;
        }

        //----------------------------------------------------------------
        
      }
      
    }
  }





