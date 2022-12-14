#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

extern int veloctiy;

void motor_stop(int left_a, int left_b, int right_a, int right_b);

void motor_left(int left_a, int left_b, int right_a, int right_b, int velocity_left, int velocity_right);

void motor_right(int left_a, int left_b, int right_a, int right_b, int velocity_left, int velocity_right);

void motor_forward(int left_a, int left_b, int right_a, int right_b, int velocity_left, int velocity_right);

void motor_reverse(int left_a, int left_b, int right_a, int right_b, int velocity_left, int velocity_right);

void set_veloctiy(int value);

void set_pins(int left_a, int left_b, int right_a, int right_b);

void motor_forward_val(int left_a, int left_b, int right_a, int right_b, int rvel, int lvel);

void motor_reverse_val(int left_a, int left_b, int right_a, int right_b, int rvel, int lvel);

#endif