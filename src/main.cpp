#include <Arduino.h>
#include "MotorControl.hpp"

#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
#include "../lib/QTRSensor/QTRSensors.h"
#include <ESP32Servo.h>
// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Ball-E connect"
#define REMOTEXY_ACCESS_PASSWORD "#DasBesteEsp2022!"

// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 92 bytes
  { 255,4,0,0,0,85,0,16,26,1,10,48,40,76,15,15,4,26,31,79,
  78,0,31,79,70,70,0,2,0,8,78,22,11,190,26,31,31,79,80,69,
  78,0,67,76,79,83,69,0,5,32,7,10,50,50,190,26,31,129,0,8,
  69,21,4,190,66,97,108,108,32,109,111,117,110,116,0,129,0,39,69,17,
  4,190,65,117,116,111,112,105,108,111,116,0 };

// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t pushSwitch_1; // =1 if state is ON, else =0 
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  int8_t joystick_1_x; // from -100 to 100  
  int8_t joystick_1_y; // from -100 to 100  

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

//initialize the motor control and sensor
enum{
  servo_pin = 19,
  Motor_links_A = 3,
  Motor_links_B = 2,
  Motor_rechts_A = 1,
  Motor_rechts_B = 0,
  sensor_right = 5,
  sensor_left = 4,
  PIN_PUSHSWITCH_1 = 8,
};
bool autopilot = false;

int red_lamp = 6;
int green_lamp = 7;

int joystick (int xy);
int set_control_mode();
void control_servo();

Servo servo;
int servo_val = 0;

void setup() {
  RemoteXY_Init ();
  Serial.begin(250000);

  pinMode(red_lamp, OUTPUT);
  pinMode(green_lamp, OUTPUT);

  pinMode (servo_pin, OUTPUT);
  //pinMode (PIN_SWITCH_2, OUTPUT);

  servo.setPeriodHertz(50); // standard 50 hz servo
  servo.attach(servo_pin); // attaches the servo on pin 19 to the servo object

  set_pins(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
}

int readSensor(int sensor_right, int sensor_left) {
  //delay(100);
  int sensor_right_value = analogRead(sensor_right);
  int sensor_left_value = analogRead(sensor_left);
  Serial.println(sensor_right_value);
  Serial.println(sensor_left_value);

  // if both sensors are on black
  if(sensor_right_value > 4000 && sensor_left_value > 4000) {
    return 1;
  } else if(sensor_right_value > 4000 && sensor_left_value < 4000) {
    return 2;
  } else if(sensor_right_value < 4000 && sensor_left_value > 4000) {
    return 3;
  } else if(sensor_right_value < 4000 && sensor_left_value < 4000) {
    return 4;
  } else {
    return 0;
  }
}

void loop() {
  RemoteXY_Handler ();
  // tells what the sensor is reading and what to do
  control_servo();
  if(set_control_mode()) {
    switch (readSensor(sensor_right, sensor_left)) {
      case 1:
        motor_forward(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, 215, 215);  
        digitalWrite(red_lamp, HIGH); // FORWARD = RED & GREEN
        digitalWrite(green_lamp, HIGH);
        break;
      case 2:
        motor_left(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, 215, 215);
        digitalWrite(red_lamp, HIGH); // LEFT = RED
        digitalWrite(green_lamp, LOW);
        break;
      case 3:
        motor_right(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, 215, 215);
        digitalWrite(red_lamp, LOW);  // RIGHT = GREEN
        digitalWrite(green_lamp, HIGH);
        break;
      case 4:
        motor_reverse(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, 215, 215);
        digitalWrite(red_lamp, LOW);  // Reverse = NEITHER
        digitalWrite(green_lamp, LOW);
        break;
      default:
        break;
    };
  } else {
      // engine right
      int right_motor_speed = joystick(RemoteXY.joystick_1_y - RemoteXY.joystick_1_x);

      // engine left
      int left_motor_speed = joystick(RemoteXY.joystick_1_y + RemoteXY.joystick_1_x);

    if (RemoteXY.joystick_1_y >= 0) {
        motor_forward_val (Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, right_motor_speed, left_motor_speed);
    } else if (RemoteXY.joystick_1_y < 0) {
        motor_reverse_val(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, right_motor_speed, left_motor_speed);
    } else {
        motor_stop(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
    }
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
  if(xy>100) xy=100;
  if(xy<-100) xy=-100;
 
  if(xy == 0) xy=0;

  if(xy>0) xy=(int)map(xy,0,100,175,255);
  if(xy<0) xy=(int)map(xy,-100,0,255,175);
  return xy;
}
// 275  =  15°
// 2070 =  90°
// 4095 =  165°
void control_servo() {
  // Open Servo
  if (RemoteXY.switch_1 == 1 && servo_val == 0) {
    servo_val = 1;
    Serial.printf("Servo: %d\n", servo_val);
    servo.write(40);
  }
  // Close Servo
  if (RemoteXY.switch_1 == 0 && servo_val == 1) {
    servo_val = 0;
    Serial.printf("Servo: %d\n", servo_val);
    servo.write(165);
  }
}