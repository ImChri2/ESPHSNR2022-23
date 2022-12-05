#include<Arduino.h>

int velocity = 235;

// Halte den Roboter an
void motor_stop(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, LOW);
  analogWrite(left_b, LOW);
  analogWrite(right_a, LOW);
  analogWrite(right_b, LOW);
}

// Bewege den Roboter nach links
void motor_left(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, LOW);
  analogWrite(left_b, velocity);
  analogWrite(right_a, LOW);
  analogWrite(right_b, velocity);
}

// Bewege den Roboter nach rechts
void motor_right(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, velocity);
  analogWrite(left_b, LOW);
  analogWrite(right_a, velocity);
  analogWrite(right_b, LOW);
}

// Bewege den Roboter nach vorne
void motor_forward(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, velocity);            //LEFT_motor takes reverse inputs
  analogWrite(left_b, LOW);                 //Truth table flipped for left_a/b
  analogWrite(right_a, LOW);
  analogWrite(right_b, velocity);
}

void motor_reverse(int left_a, int left_b, int right_a, int right_b) {
  analogWrite(left_a, LOW);            //LEFT_motor takes reverse inputs
  analogWrite(left_b, velocity);                 //Truth table flipped for left_a/b
  analogWrite(right_a, velocity);
  analogWrite(right_b, LOW);
}

// Setze die Geschwindigkeit
void set_velocity(int value) {
  velocity = value;
}

// Einlesen der Pins für die Motoren
void set_pins(int left_a, int left_b, int right_a, int right_b) {
  pinMode(left_a, OUTPUT);
  pinMode(left_b, OUTPUT);
  pinMode(right_a, OUTPUT);
  pinMode(right_b, OUTPUT);
  digitalWrite(left_a, HIGH);
  digitalWrite(left_b, HIGH);
  digitalWrite(right_a, HIGH);
  digitalWrite(right_b, HIGH);
}

// Bewege den Roboter nach links
void motor_left_val(int left_a, int left_b, int right_a, int right_b, int rvel, int lvel) {
  analogWrite(left_a, LOW);
  analogWrite(left_b, lvel);
  analogWrite(right_a, LOW);
  analogWrite(right_b, rvel);
}

// Bewege den Roboter nach rechts
void motor_right_val(int left_a, int left_b, int right_a, int right_b, int rvel, int lvel) {
  analogWrite(left_a, lvel);
  analogWrite(left_b, LOW);
  analogWrite(right_a, rvel);
  analogWrite(right_b, LOW);
}

// Bewege den Roboter nach vorne
void motor_forward_val(int left_a, int left_b, int right_a, int right_b, int rvel, int lvel) {
  analogWrite(left_a, lvel);            //LEFT_motor takes reverse inputs
  analogWrite(left_b, LOW);                 //Truth table flipped for left_a/b
  analogWrite(right_a, LOW);
  analogWrite(right_b, rvel);
}

void motor_reverse_val(int left_a, int left_b, int right_a, int right_b, int rvel, int lvel) {
  analogWrite(left_a, LOW);            //LEFT_motor takes reverse inputs
  analogWrite(left_b, lvel);                 //Truth table flipped for left_a/b
  analogWrite(right_a, rvel);
  analogWrite(right_b, LOW);
}