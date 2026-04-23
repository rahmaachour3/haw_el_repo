#include "motor.h"

void init_motor_pins() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(StbyPin, OUTPUT);

  // Configuration PWM ESP32 (API v2.x)
  ledcSetup(0, 20000, 8); // Canal 0
  ledcSetup(1, 20000, 8); // Canal 1
  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);

  // Activation du driver
  digitalWrite(StbyPin, HIGH);
}
void setMotorLeft(int speed) {
  if (speed >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    speed = -speed;
  }
  ledcWrite(0, constrain(speed, 0, 255));
}

void setMotorRight(int speed) {
  if (speed >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    speed = -speed;
  }
  ledcWrite(1, constrain(speed, 0, 255));
}
void motorStop()
{
  digitalWrite(StbyPin,LOW);
}
void moveForward(int speed) {
  setMotorLeft(speed);
  setMotorRight(speed);
}


void moveBackward(int speed) {
  setMotorLeft(-speed);
  setMotorRight(-speed);
}


void turnLeft(int speed) {
  setMotorLeft(-speed);
  setMotorRight(speed);
}


void turnRight(int speed) {
  setMotorLeft(speed);
  setMotorRight(-speed);
}

