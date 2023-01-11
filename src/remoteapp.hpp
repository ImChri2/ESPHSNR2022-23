#ifndef REMOTEAPP_H
#define REMOTEAPP_H

int readSensor(int sensor_right, int sensor_left);
int joystick (int xy);
int set_control_mode(bool autopilot);
void control_servo(bool servo_val);

#endif