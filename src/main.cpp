#include <Arduino.h>
#include "MotorControl.hpp"
#include "Autopilot.cpp"

#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "ESP2022_Slider"
#define REMOTEXY_ACCESS_PASSWORD "esp2022"
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
 uint8_t pushSwitch_Engine; // =1 if state is ON, else =0
 uint8_t switch_2; // =1 if switch ON and =0 if OFF
 int8_t slider_links; // =-100..100 slider position
 int8_t slider_rechts; // =-100..100 slider position
 // other variable
 uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)

int Motor_links_A = 3;
int Motor_links_B = 2;
int Motor_rechts_A = 1;
int Motor_rechts_B = 0;
int Sensor_a = 4;
int Sensor_b = 5;
bool autopilot = false;



void setup() {
  RemoteXY_Init ();
  Serial.begin(9600);

  set_pins(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
}

int readSensor(int sensor_a, int sensor_b) {
  delay(100);
  int sensor_a_value = analogRead(sensor_a);
  int sensor_b_value = analogRead(sensor_b);
  Serial.println(sensor_a_value);
  Serial.println(sensor_b_value);

  if(sensor_a_value > 1000 && sensor_b_value > 1000) {
    return 0;
  } else if(sensor_a_value > 1000 && sensor_b_value < 1000) {
    return 1;
  } else if(sensor_a_value < 1000 && sensor_b_value > 1000) {
    return 2;
  } else if(sensor_a_value < 1000 && sensor_b_value < 1000) {
    return 3;
  } else {
    return 0;
  }
}

void loop() {
  RemoteXY_Handler ();
  switch (readSensor(Sensor_a, Sensor_b)) {
    case 1:
      motor_forward(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
      break;
    case 2:
      motor_left(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
      break;
    case 3:
      motor_right(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
      break;
    case 4:
      motor_stop(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
      break;
    default:
      break;
  };
}

void set_control_mode() {
  if (RemoteXY.pushSwitch_Engine == 1) {
    autopilot = true;
  } else {
    autopilot = false;
  }
}