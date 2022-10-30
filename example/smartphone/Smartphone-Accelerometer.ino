/*
   -- ESP2022_Accelerometer --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.8 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.1 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "ESP2022_Slider"


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 92 bytes
  { 255,15,0,11,0,85,0,16,30,1,2,1,2,73,34,11,2,26,31,16,
  80,104,111,110,101,0,83,101,110,115,111,114,0,67,4,3,63,55,5,2,
  26,11,9,0,4,6,54,54,24,10,48,38,72,22,22,4,50,8,71,79,
  0,8,83,84,79,80,0,2,1,2,89,34,8,2,26,31,31,77,111,116,
  46,79,78,0,77,111,116,46,79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  float accelerometer_1_x;
  float accelerometer_1_y;
  float accelerometer_1_z;
  uint8_t pushSwitch_1; // =1 if state is ON, else =0 
  uint8_t switch_2; // =1 if switch ON and =0 if OFF 

    // output variables
  char text_1[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_PUSHSWITCH_1 10
#define PIN_SWITCH_2 9


void setup() 
{
  RemoteXY_Init (); 
  
  pinMode (PIN_PUSHSWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_PUSHSWITCH_1, (RemoteXY.pushSwitch_1==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_2, (RemoteXY.switch_2==0)?LOW:HIGH);
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}
