#include <Arduino.h>
#include "odometry.h"
#include "motor.h"
#include "ros_communication.h"
RosCommunication ros_communication;

void setup() {
  init_motor_pins();
  ros_communication.initialize();
  ros_communication.subscriber_define();
  ros_communication.odom_define(); 
  ros_communication.executors_start();
}

void loop() {
  updatePosition();
  ros_communication.publish_odom(
    current_pose.x, current_pose.y, current_pose.theta,
    current_velocity.linear, current_velocity.angular
  );
  ros_communication.start_receiving_msgs();
  delay(100); // Ajustez ce délai selon vos besoins
}