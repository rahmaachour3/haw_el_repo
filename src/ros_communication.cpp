#include "ros_communication.h"
#include "motor.h"
#include "odometry.h"
#include <math.h>
#include <SPI.h>

// ── ROS entities ─────────────────────────────────
rcl_subscription_t cmd_vel_sub;
geometry_msgs__msg__Twist cmd_vel_msg;

rcl_publisher_t odom_publisher;
nav_msgs__msg__Odometry odom_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

// ── Display ──────────────────────────────────────
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ── Frame IDs ────────────────────────────────────
static char odom_frame_id[] = "odom";
static char odom_child_frame_id[] = "base_link";

RosCommunication::RosCommunication() {}

// ─────────────────────────────────────────────────
void RosCommunication::initialize() {
    Serial.begin(115200);
    Serial.println("ROS Communication init");

    // IMPORTANT: correct WiFi transport
    char ssid[] = "Jhelum.net [Luqman House]";
    char psk[]  = "7861234786";

    IPAddress agent_ip(192, 168, 100, 21);   // MUST match PC running agent
    size_t agent_port = 8888;

    set_microros_wifi_transports(ssid, psk, "1.1.1.1", agent_port);

    delay(2000);

    allocator = rcl_get_default_allocator();

    rclc_support_init(&support, 0, NULL, &allocator);
    rclc_node_init_default(&node, "esp32_robot_node", "", &support);
}

// ─────────────────────────────────────────────────
void RosCommunication::subscriber_define() {
    rclc_subscription_init_default(
        &cmd_vel_sub,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        "/cmd_vel"
    );
}

// ─────────────────────────────────────────────────
void RosCommunication::odom_define() {
    rclc_publisher_init_default(
        &odom_publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
        "/odom"
    );

    odom_msg.header.frame_id.data = odom_frame_id;
    odom_msg.header.frame_id.size = strlen(odom_frame_id);
    odom_msg.header.frame_id.capacity = sizeof(odom_frame_id);

    odom_msg.child_frame_id.data = odom_child_frame_id;
    odom_msg.child_frame_id.size = strlen(odom_child_frame_id);
    odom_msg.child_frame_id.capacity = sizeof(odom_child_frame_id);
}

// ─────────────────────────────────────────────────
void RosCommunication::executors_start() {
    rclc_executor_init(&executor, &support.context, 1, &allocator);

    rclc_executor_add_subscription(
        &executor,
        &cmd_vel_sub,
        &cmd_vel_msg,
        &RosCommunication::cmd_vel_callback,
        ON_NEW_DATA
    );
}

// ─────────────────────────────────────────────────
void RosCommunication::publish_odom(
    float x, float y,
    float theta,
    float linear_vel,
    float angular_vel)
{
    uint32_t now_ms = millis();

    odom_msg.header.stamp.sec = now_ms / 1000;
    odom_msg.header.stamp.nanosec = (now_ms % 1000) * 1000000UL;

    odom_msg.pose.pose.position.x = x;
    odom_msg.pose.pose.position.y = y;
    odom_msg.pose.pose.position.z = 0.0;

    odom_msg.pose.pose.orientation.x = 0.0;
    odom_msg.pose.pose.orientation.y = 0.0;
    odom_msg.pose.pose.orientation.z = sinf(theta / 2.0f);
    odom_msg.pose.pose.orientation.w = cosf(theta / 2.0f);

    odom_msg.twist.twist.linear.x = linear_vel;
    odom_msg.twist.twist.angular.z = angular_vel;

    rcl_publish(&odom_publisher, &odom_msg, NULL);
}

// ─────────────────────────────────────────────────
void RosCommunication::cmd_vel_callback(const void *msg_recv) {
    const geometry_msgs__msg__Twist *msg =
        (const geometry_msgs__msg__Twist *)msg_recv;

    float linear = msg->linear.x;
    float angular = msg->angular.z;

    Serial.print(linear);
    Serial.print(" / ");
    Serial.println(angular);

    // motor control (unchanged)
   /* if (linear > 0) forward(linear * 255);
    else if (linear < 0) reverse(-linear * 255);
    else if (angular > 0) left(angular * 255);
    else if (angular < 0) right(-angular * 255);
    else stop();*/
}

// ─────────────────────────────────────────────────
void RosCommunication::start_receiving_msgs() {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    delay(10);
}