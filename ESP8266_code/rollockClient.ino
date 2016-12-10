  #include <Wire.h>           // To Be Able To Use Sensors Like IMU9050
  #include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
  #include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU


  // WIFI Authentication Variables
  char* TKDssid; //WIFI
  char* TKDpassword;
  char message;
  
 // WIFI Module Role & Port
  int             TKDServerPort  = 9001;
  IPAddress       TKDServer(192,168,4,1);
  WiFiClient      TKDClient;

  void setup()
  {
// Setting The Serial Port ----------------------------------------------
    Serial.begin(9600);           // Computer Communication
    pinMode(LED_BUILTIN, OUTPUT);          // Indicator For Client #1 Connectivity
     
     // Print Message Of I/O Setting Progress --------------------------------
    //Serial.println("\nI/O Pins Modes Set .... Done");





        // Starting To Connect --------------------------------------------------
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.begin("TAKEONE");          // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    //Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

    // WiFi Connectivity ----------------------------------------------------
    CheckWiFiConnectivity();        // Checking For Connection

    // Stop Blinking To Indicate Connected ----------------------------------
    digitalWrite(LED_BUILTIN, !HIGH);
    //Serial.println("!-- Client Device Connected --!");

    // Printing IP Address --------------------------------------------------
    //Serial.println("Connected To      : " + String(WiFi.SSID()));
    //Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    //Serial.print  ("Server IP Address : ");
    //Serial.println(TKDServer);
    //Serial.print  ("Device IP Address : ");
    //Serial.println(WiFi.localIP());
    
    // Conecting The Device As A Client -------------------------------------
    TKDRequest();
  }   
  
    void loop()
  {
    ReadMotor();
  }
  
  void ReadMotor()
  {
    // Reading The Button
    if(Serial.available())
    {
      message=Serial.read();
      //Serial.print(message);
      TKDClient.print(message);
      TKDClient.flush();
    }
  }


    void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(LED_BUILTIN, !HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, !LOW);
        delay(250);
       // Serial.print(".");
      }
      //Serial.println("");
    }
  }

  
  void TKDRequest()
  {
    // First Make Sure You Got Disconnected
    TKDClient.stop();

    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, TKDServerPort))
    {
      //Serial.println    ("CONNECTED");
      //TKDClient.println ("CONNECTED");
    }
  }
