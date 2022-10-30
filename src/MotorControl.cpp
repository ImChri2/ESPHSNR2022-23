#include<Arduino.h>

int val = 128;

void motor_stop(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, 0);
  analogWrite(left_b, 0);
  analogWrite(right_a, 0);
  analogWrite(right_b, 0);
}

void motor_left(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, 0);
  analogWrite(left_b, val);
  analogWrite(right_a, val);
  analogWrite(right_b, 0);
}

void motor_right(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, val);
  analogWrite(left_b, 0);
  analogWrite(right_a, 0);
  analogWrite(right_b, val);
}

void motor_forward(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, val);
  analogWrite(left_b, 0);
  analogWrite(right_a, val);
  analogWrite(right_b, 0);
}

void set_val(int value) {
  val = value;
}

void set_pins(int left_a, int left_b, int right_a, int right_b) {
  pinMode(left_a, OUTPUT);
  pinMode(left_b, OUTPUT);
  pinMode(right_a, OUTPUT);
  pinMode(right_b, OUTPUT);
}
