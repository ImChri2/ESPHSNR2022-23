#ifndef STRUCT_H
#define STRUCT_H
#include <Arduino.h>
#pragma pack(push)
#pragma pack(1)

struct {
  uint8_t pushSwitch_1; // =1 if state is ON, else =0 -> Schalter für Autopilot
  uint8_t switch_1; // =1 if switch ON and =0 if OFF -> Schalter für Servo Motor
  int8_t joystick_1_x; // Von -100 bis 100   
  int8_t joystick_1_y; // Von -100 bis 100  
  uint8_t connect_flag;  // =1 if wire connected, else =0 
} RemoteXY;

#pragma pack(pop)
#endif