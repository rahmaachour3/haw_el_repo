#ifndef ODOMETRY_H
#define ODOMETRY_H

#include <Arduino.h>

const float WHEEL_RADIUS = 33.0;    // mm
const float WHEEL_BASE   = 176.9;   // mm
const int   CPR = 100;              // counts per revolution

typedef struct {
    double x;
    double y;
    double theta;
} robot_pose_t;

typedef struct {
    double linear;   // mm/s
    double angular;  // rad/s
} robot_velocity_t;

// Variables globales
extern robot_pose_t current_pose;
extern robot_velocity_t current_velocity;

// Variables pour encodeurs (déclarées dans main)
extern volatile long enc_left;
extern volatile long enc_right;

// Fonctions
double normalizeAngle(double angle);
void updatePosition();

#endif
