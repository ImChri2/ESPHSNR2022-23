#include <Arduino.h>
#include "MotorControl.hpp"
#include "Autopilot.cpp"


#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
#include <time.h>
//#include "../lib/QTRSensor/QTRSensors.h"
// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Ball-E connect"
#define REMOTEXY_ACCESS_PASSWORD "#DasBesteEsp2022!"
// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 96 bytes
  { 255,5,0,11,0,89,0,16,29,1,2,1,3,71,29,11,2,26,31,31,
  83,101,110,115,111,114,0,80,104,111,110,101,0,67,4,3,62,55,5,2,
  26,11,5,32,4,4,55,55,2,26,31,10,48,38,74,19,19,4,36,8,
  32,71,79,32,0,24,83,84,79,80,0,2,1,3,89,29,7,2,26,31,
  31,77,111,116,46,79,78,0,77,111,116,46,79,70,70,0 };


int joystick (int xy);
int set_control_mode();


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

struct motor_t {
  uint8_t motor; // 0 left, 1 right
  int8_t direction; // 0 forward, 1 backward, 2 left, 3 right
  unsigned left_speed; // 0-255
  unsigned right_speed; // 0-255
};
motor_t motor;
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

void calc_speeds(motor_t * motor, int right, int left) {
  /*if (right < 200) {
    right = 200;
  }
  if (left < 200) {
    left = 200;
  }
  // Look up the motor speeds using the sensor values as indices
  //int right_motor_speed = map(right,4095,400,175,255);
  //int left_motor_speed = map(left,4095,400,175,255);*/
  int left_time_since_last_black_contact = 0;
  int right_time_since_last_black_contact = 0;
  int right_motor_speed;
  int left_motor_speed;
    // Get current time in milliseconds
  time_t now = time(NULL);
  //strftime(now, "%Y-%m-%d %H:%M:%S", t);
  struct tm *t = localtime(&now);

  // Format time in milliseconds
  char buf[128];
  strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S.%f", t);
  Serial.printf("Current time: %s\n", buf);
  
  if(right > 1000 || left > 1000) {
    right_motor_speed = right > 1000 ? 255 : right_motor_speed;
    left_motor_speed = left > 1000 ? 255 : left_motor_speed;
  }
  /*TIme idea testing
  if(right < 900 || left < 900) {
    right < 900 ? right_time_since_last_black_contact++ : right_time_since_last_black_contact == 0;
    left < 900 ? left_time_since_last_black_contact++ : left_time_since_last_black_contact == 0;
  }*/
  if(right > 600 || left > 600) {
    right_motor_speed = right > 800 ? 250 : right_motor_speed;
    left_motor_speed = left > 800 ? 250 : left_motor_speed;
  }
  if(right > 400 || left > 400) {
    right_motor_speed = right > 400 ? 220 : right_motor_speed;
    left_motor_speed = left > 400 ? 220 : left_motor_speed;
  }
  if(right > 200 || left > 200) {
    right_motor_speed = right > 200 ? 200 : right_motor_speed;
    left_motor_speed = left > 200 ? 200 : left_motor_speed;
  }

  // Return the motor speeds
  motor->left_speed = left_motor_speed;
  motor->right_speed = right_motor_speed;
}

int readSensor(int sensor_right, int sensor_left) {
  //delay(100);
  int sensor_right_value = analogRead(sensor_right);
  int sensor_left_value = analogRead(sensor_left);
  Serial.println(sensor_right_value);
  Serial.println(sensor_left_value);

  // if both sensors are on black
  if(sensor_right_value > 1000 && sensor_left_value > 1000) {
    return motor.direction = 1;
  } else if(sensor_right_value > 1000 && sensor_left_value < 1000) {
    return motor.direction = 2;
  } else if(sensor_right_value < 1000 && sensor_left_value > 1000) {
    return motor.direction = 3;
  } else if(sensor_right_value < 1000 && sensor_left_value < 1000) {
    return motor.direction = 4;
  } else {
    return 0;
  }
}


void loop() {
  RemoteXY_Handler ();
  // tells what the sensor is reading and what to do
  if(set_control_mode()) {
    calc_speeds(&motor, sensor_right, sensor_left);
    switch (readSensor(sensor_right, sensor_left)) {
      case 1:
        motor_forward(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, motor.left_speed, motor.right_speed);  
        digitalWrite(red_lamp, HIGH); // FORWARD = RED & GREEN
        digitalWrite(green_lamp, HIGH);
        break;
      case 2:
        motor_left(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, motor.left_speed, motor.right_speed);
        digitalWrite(red_lamp, HIGH); // LEFT = RED
        digitalWrite(green_lamp, LOW);
        break;
      case 3:
        motor_right(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, motor.left_speed, motor.right_speed);
        digitalWrite(red_lamp, LOW);  // RIGHT = GREEN
        digitalWrite(green_lamp, HIGH);
        break;
      case 4:
        motor_reverse(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B, motor.left_speed, motor.right_speed);
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

