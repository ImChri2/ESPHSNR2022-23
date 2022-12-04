#include <Arduino.h>
#include "MotorControl.hpp"
#include "Autopilot.cpp"

#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Ball-E connect"
#define REMOTEXY_ACCESS_PASSWORD "#DasBesteEsp2022!"
// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 94 bytes
 { 255,5,0,0,0,87,0,16,29,1,2,1,3,71,29,11,2,26,31,31,
 83,101,110,115,111,114,0,80,104,111,110,101,0,10,48,38,74,19,19,4,
 36,8,32,71,79,32,0,24,83,84,79,80,0,2,1,3,88,29,8,2,
 26,31,31,77,111,116,46,79,78,0,77,111,116,46,111,102,102,0,4,48,
 10,3,10,64,2,26,4,48,40,3,10,64,2,26 };

// this structure defines all the variables and events of your control interface
struct {
    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  int8_t joystick_1_x; // from -100 to 100  
  int8_t joystick_1_y; // from -100 to 100  
  uint8_t pushSwitch_1; // =1 if state is ON, else =0 
  uint8_t switch_2; // =1 if switch ON and =0 if OFF 

    // output variables
  char text_1[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

#define PIN_PUSHSWITCH_1 10
#define PIN_SWITCH_2 9
//initialize the motor control and sensor
int Motor_links_A = 3;
int Motor_links_B = 2;
int Motor_rechts_A = 1;
int Motor_rechts_B = 0;
int sensor_right = 5;
int sensor_left = 4;
bool autopilot = false;

int red_lamp = 6;
int green_lamp = 7;



void setup() {
  RemoteXY_Init ();
  Serial.begin(250000);

  pinMode(red_lamp, OUTPUT);
  pinMode(green_lamp, OUTPUT);

  pinMode (PIN_PUSHSWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  

  set_pins(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
}

int readSensor(int sensor_right, int sensor_left) {
  //delay(100);
  int sensor_right_value = analogRead(sensor_right);
  int sensor_left_value = analogRead(sensor_left);
  Serial.println(sensor_right_value);
  Serial.println(sensor_left_value);

  // if both sensors are on black
  if(sensor_right_value > 1000 && sensor_left_value > 1000) {
    return 1;
  } else if(sensor_right_value > 1000 && sensor_left_value < 1000) {
    return 2;
  } else if(sensor_right_value < 1000 && sensor_left_value > 1000) {
    return 3;
  } else if(sensor_right_value < 1000 && sensor_left_value < 1000) {
    return 4;
  } else {
    return 0;
  }
}


void loop() {
  RemoteXY_Handler ();
  // tells what the sensor is reading and what to do
  if(set_control_mode())
  {
    switch (readSensor(sensor_right, sensor_left)) {
      case 1:
        motor_forward(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
        digitalWrite(red_lamp, HIGH);
        digitalWrite(green_lamp, HIGH);
        break;
      case 2:
        motor_left(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
        digitalWrite(red_lamp, HIGH);
        digitalWrite(green_lamp, LOW);
        break;
      case 3:
        motor_right(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
        digitalWrite(red_lamp, LOW);
        digitalWrite(green_lamp, HIGH);
        break;
      case 4:
        motor_reverse(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
        digitalWrite(red_lamp, LOW);
        digitalWrite(green_lamp, LOW);
        break;
      default:
        break;
    };
  }
  else {
    // engine right
    set_veloctiy(joystick(RemoteXY.joystick_1_y - RemoteXY.joystick_1_x));

    // engine left
    set_veloctiy(joystick(RemoteXY.joystick_1_y + RemoteXY.joystick_1_x));
    
  }
}

int set_control_mode() {
  if (RemoteXY.pushSwitch_1 == 1) {
    autopilot = true;
  } else {
    autopilot = false;
  }
  return autopilot;
}

int joystick (int xy) {
  if(xy>200) xy=200;
  if(xy<-200) xy=-200;
 
  if(xy == 0) xy=0;

  if(xy>0) xy=map(xy,0,200,150,255);
  if(xy<0) xy=map(xy,-200,0,-255,-150);
  return xy;
}