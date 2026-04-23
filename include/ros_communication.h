#ifndef ROS_COMMUNICATION_H
#define ROS_COMMUNICATION_H

#include <micro_ros_arduino.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>
#include <nav_msgs/msg/odometry.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>
// ── Fix 1: Add the correct display library ────────────────────────────────────
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// ── Fix 2: Declare the global display so cmd_vel_callback (static) can see it ─
extern Adafruit_SSD1306 display;

class RosCommunication {
public:
    RosCommunication();
    void initialize();
    void subscriber_define();
    void odom_define();
    void publish_odom(float x, float y, float theta, float linear_vel, float angular_vel);
    void executors_start();
    void start_receiving_msgs();

    // Fix 3: static — required to be passed as a C function pointer to rclc
    static void cmd_vel_callback(const void *msg_recv);

private:
    // Fix 4: Remove these — cmd_vel values are read directly inside the
    // callback and passed straight to motor functions; storing them as
    // static members serves no purpose and risks stale values being used
    // static float linear_vel;   ← removed
    // static float angular_vel;  ← removed
};

#endif