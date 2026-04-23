#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h> // Nécessaire pour les types (int, byte, etc.)

// --- DÉFINITION DES PINS ---

#define ENA  25    // PWM gauche
#define IN1  16
#define IN2  17

#define ENB  26    // PWM droit
#define IN3  18
#define IN4  19
#define StbyPin 27

// Ces fonctions seront accessibles depuis le main
void init_motor_pins();
void setMotorLeft(int speed);
void setMotorRight(int speed);
void motorStop();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);

#endif