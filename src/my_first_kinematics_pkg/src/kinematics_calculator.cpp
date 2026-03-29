#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <Eigen/Dense>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>           // NEW: For publishing odometry
#include <tf2/LinearMath/Quaternion.h>        // NEW: For orientation

using namespace std::chrono_literals;

class KinematicsNode : public rclcpp::Node
{
public:
  KinematicsNode() : Node("kinematics_node")
  {
    RCLCPP_INFO(this->get_logger(), "🚀 Phase 2: Kinematics Node with Odometry started!");

    // Publishers
    cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    odom_publisher_    = this->create_publisher<nav_msgs::msg::Odometry>("odom", 10);

    // Timer at 20 Hz
    timer_ = this->create_wall_timer(
      50ms,
      std::bind(&KinematicsNode::control_loop, this));
  }

private:
  void control_loop()
  {
    // === 1. Wheel velocities (simulate encoder data) ===
    double v_left  = 0.4;   // m/s
    double v_right = 0.6;   // m/s
    double wheelbase = 0.35; // meters

    // === 2. Forward Kinematics ===
    double linear_vel  = (v_right + v_left) / 2.0;
    double angular_vel = (v_right - v_left) / wheelbase;

    // === 3. Update Robot Pose (Position Integration) ===
    double dt = 0.05;  // 50ms timer = 0.05 seconds

    // Update orientation first
    theta_ += angular_vel * dt;

    // Update position using current heading
    x_ += linear_vel * std::cos(theta_) * dt;
    y_ += linear_vel * std::sin(theta_) * dt;

    // === 4. Create and publish cmd_vel ===
    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.linear.x = linear_vel;
    cmd_vel.angular.z = angular_vel;
    cmd_vel_publisher_->publish(cmd_vel);

    // === 5. Create and publish Odometry ===
    nav_msgs::msg::Odometry odom;
    odom.header.stamp = this->now();           // Current ROS time
    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";

    // Position
    odom.pose.pose.position.x = x_;
    odom.pose.pose.position.y = y_;
    odom.pose.pose.position.z = 0.0;

    // Orientation (convert theta to quaternion)
    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, theta_);
    odom.pose.pose.orientation.x = q.x();
    odom.pose.pose.orientation.y = q.y();
    odom.pose.pose.orientation.z = q.z();
    odom.pose.pose.orientation.w = q.w();

    // Velocity
    odom.twist.twist.linear.x = linear_vel;
    odom.twist.twist.angular.z = angular_vel;

    odom_publisher_->publish(odom);

    // Log progress
    RCLCPP_INFO(this->get_logger(),
      "Pose: (x=%.2f, y=%.2f, θ=%.2f rad) | Vel: v=%.2f m/s, ω=%.2f rad/s",
      x_, y_, theta_, linear_vel, angular_vel);
  }

  // === Member Variables (Robot State) ===
  double x_ = 0.0;
  double y_ = 0.0;
  double theta_ = 0.0;   // orientation in radians

  // Publishers
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;

  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<KinematicsNode>());
  rclcpp::shutdown();
  return 0;
}