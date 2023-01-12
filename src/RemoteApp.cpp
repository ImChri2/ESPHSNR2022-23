#include <Arduino.h>
#include <Struct.hpp>
#include <ESP32Servo.h>
#include <BLEDevice.h>
#include <RemoteXY.h>

extern Servo servo;

/**
*
*    @brief reads sensor values and returns a code depending on the sensor values
*
*    @param sensor_right the pin number of the right sensor
*
*    @param sensor_left the pin number of the left sensor
*
*    @return int code indicating the sensor values
*
*    @retval 0 if both sensors are not on black
*
*    @retval 1 if both sensors are on black
*
*    @retval 2 if only the right sensor is on black
*
*    @retval 3 if only the left sensor is on black
*
*    @retval 4 if both sensors are not on black
*/
int readSensor(int sensor_right, int sensor_left) {
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


/**
* @brief Die Funktion joystick() normalisiert die Eingabe des Joysticks und gibt die übertragene Geschwindigkeit zurück.
* 
* Die Funktion erhält einen Wert von xy, der die Position des Joysticks auf der x- oder y-Achse repräsentiert.
* Der Wert von xy wird zunächst begrenzt, damit er innerhalb des Wertebereichs von -100 bis 100 liegt.
* Danach wird der Wert über die map() Funktion auf einen Bereich von 175 bis 255 normalisiert.
* 
* @param xy Position des Joysticks auf der x- oder y-Achse
* 
* @return Normalisierter Wert für die Geschwindigkeit
*/
int joystick (int xy) {
  if(xy>100) xy=100;
  if(xy<-100) xy=-100;
 
  if(xy == 0) xy=0;

  if(xy>0) xy=(int)map(xy,0,100,175,255);
  if(xy<0) xy=(int)map(xy,-100,0,255,175);
  return xy;
}


/**
* @brief Die Funktion set_control_mode() prüft den Status des Schalters "pushSwitch_1" auf dem Fernbedienungsgerät und gibt den entsprechenden Autopilot-Modus zurück.
* 
* Die Funktion überprüft den Wert von RemoteXY.pushSwitch_1. 
* Falls der Wert 1 ist, wird die variable "autopilot" auf "true" gesetzt und der Autopilot-Modus aktiviert.
* 
* Ansonsten wird "autopilot" auf "false" gesetzt und der Autopilot-Modus deaktiviert.
* 
* @return Autopilot-Modus (true oder false)
*/
int set_control_mode(bool autopilot) {
  if(RemoteXY.pushSwitch_1 == 1) {
    autopilot = true;
  } else {
    autopilot = false;
  }
  return autopilot;
}

/**
 * @brief Steuerung des Servo Motors
 * 
 * In der Funktion wird überprüft, ob der Schalte in der App auf Auf/Zu steht und ob der Servo bereits auf oder zu ist.
 * Jenachdem wird der Servo geöffnet oder geschlossen.
 *
 * Servo Grenzwerte: 0-180
 **/
void control_servo(bool servo_val) {
  // Open Servo
  if (RemoteXY.switch_1 == 1 && servo_val == true) {
    servo_val = true;
    Serial.printf("Servo: %d\n", servo_val);
    servo.write(50);
  }
  // Close Servo
  if (RemoteXY.switch_1 == 0 && servo_val == true) {
    servo_val = false;
    Serial.printf("Servo: %d\n", servo_val);
    servo.write(165);
  }
}