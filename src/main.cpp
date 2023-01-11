#include <Arduino.h>
#include "MotorControl.hpp"
#include "RemoteApp.hpp"
#include "../lib/QTRSensor/QTRSensors.h"
#include <ESP32Servo.h>
#include <Struct.hpp>

// RemoteXY connection settings
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
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


// Pin-Definitionen
enum{
  servo_pin = 19,
  Motor_links_A = 3,
  Motor_links_B = 2,
  Motor_rechts_A = 1,
  Motor_rechts_B = 0,
  sensor_right = 5,
  sensor_left = 4,
  PIN_PUSHSWITCH_1 = 8,
  red_lamp = 6,
  green_lamp = 7,
};
bool servo_val = false;
bool autopilot = false;

Servo servo;

/**
* @brief Die setup() Funktion initialisiert die Verbindung und Einrichtungen für die Steuerung des ferngesteuerten Fahrzeugs.
* 
* Zunächst wird die RemoteXY_Init() Funktion aufgerufen, welche die Verbindung mit dem Fernbedienungsgerät herstellt.
* 
* Anschließend wird eine serielle Verbindung mit einer Geschwindigkeit von 250000 baud () aufgebaut.
* 
* Danach werden Pin-Modi für die Ausgänge red_lamp und green_lamp sowie den Pin servo_pin festgelegt.
* 
* Außerdem wird der Perioden-Hertzwert für den Servomotor auf 50 gesetzt und der Servomotor an den Pin servo_pin angeschlossen.
* 
* Abschließend werden die Pins für die Motoren (Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B) festgelegt.
*/
void setup() {
  RemoteXY_Init ();
  Serial.begin(250000);

  pinMode(red_lamp, OUTPUT);
  pinMode(green_lamp, OUTPUT);

  pinMode (servo_pin, OUTPUT);

  servo.setPeriodHertz(50); // standard 50 hz servo
  servo.attach(servo_pin); // attaches the servo on pin 19 to the servo object

  set_pins(Motor_links_A, Motor_links_B, Motor_rechts_A, Motor_rechts_B);
}

/**
* @brief Die loop() Funktion steuert die Bewegungen und Aktionen eines ferngesteuerten Fahrzeugs.
*
* Zu Beginn wird die RemoteXY_Handler() Funktion aufgerufen, welche die Kommunikation mit dem Fernbedienungsgerät handhabt.
* 
* Anschließend wird die control_servo() Funktion aufgerufen, welche die Steuerung des Servomotors übernimmt.
* 
* Danach wird eine bedingte Anweisung ausgeführt, welche durch den Rückgabewert der set_control_mode() Funktion gesteuert wird.
* Falls diese true zurückliefert, wird eine switch Anweisung ausgeführt, welche den Rückgabewert der readSensor() Funktion auswertet.
* 
* Abhängig von diesem Wert werden verschiedene Motorfunktionen aufgerufen (motor_forward, motor_left, motor_right, motor_reverse) und unterschiedliche Werte für die Ausgänge red_lamp und green_lamp gesetzt.
* 
* Falls set_control_mode() false zurückliefert, wird ein anderer Teil des Codes ausgeführt, welcher die x- und y- Koordinaten des Joysticks von "RemoteXY" abruft und diese in Geschwindigkeiten für den linke und rechte Motor umwandelt.
* Anschließend entscheidet das Programm ob es vorwärts, rückwärts oder anhalten soll indem es den y- Koordinate Wert des Joysticks vergleicht.
*/
void loop() {
  RemoteXY_Handler ();
  // tells what the sensor is reading and what to do
  control_servo(servo_val);
  if(set_control_mode(autopilot)) {
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
      // Rechter Motor
      int right_motor_speed = joystick(RemoteXY.joystick_1_y - RemoteXY.joystick_1_x);
      // Linker Motor
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