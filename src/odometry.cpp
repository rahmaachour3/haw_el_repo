#include "odometry.h"

robot_pose_t current_pose = {0.0, 0.0, 0.0};
robot_velocity_t current_velocity = {0.0, 0.0};

unsigned long lastTime = 0; 
volatile long lastEncL = 0, lastEncR = 0;
volatile long enc_left = 0;
volatile long enc_right = 0;
double normalizeAngle(double angle) {
    while (angle > PI) angle -= 2.0 * PI;
    while (angle < -PI) angle += 2.0 * PI;
    return angle;
}

void updatePosition() {
    unsigned long currentTime = millis();
    double dt = (currentTime - lastTime) / 1000.0;
    if (dt <= 0) return;
    lastTime = currentTime;
    
    // Lecture sécurisée des encodeurs
    noInterrupts();
    long currentL = enc_left;
    long currentR = enc_right;
    interrupts();
    
    long deltaL = currentL - lastEncL;
    long deltaR = currentR - lastEncR;
    lastEncL = currentL;
    lastEncR = currentR;
    
    // Distance parcourue par chaque roue
    double dL = (2.0 * PI * WHEEL_RADIUS) * deltaL / CPR;
    double dR = (2.0 * PI * WHEEL_RADIUS) * deltaR / CPR;
    
    // Distance du centre et variation d'angle
    double dC = (dL + dR) / 2.0;
    double dTheta = (dR - dL) / WHEEL_BASE;
    
    // CORRECTION: Calcul de la vitesse (c'était l'erreur dans votre code)
    current_velocity.linear = dC / dt;
    current_velocity.angular = dTheta / dt;
    
    // Mise à jour de la pose
    current_pose.x += dC * cos(current_pose.theta + dTheta / 2.0);
    current_pose.y += dC * sin(current_pose.theta + dTheta / 2.0);
    current_pose.theta += dTheta;
    current_pose.theta = normalizeAngle(current_pose.theta);
}