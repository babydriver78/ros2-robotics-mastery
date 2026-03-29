#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <Eigen/Dense>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <tf2/LinearMath/Quaternion.h>

using namespace std::chrono_literals;

class KinematicsNode : public rclcpp::Node
{
public:
  KinematicsNode() : Node("kinematics_node")
  {
    RCLCPP_INFO(this->get_logger(), "🚀 Improved Kinematics Node with Variable Control started!");

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
    // === Variable Control: Change wheel velocities over time ===
    // Using sine wave to simulate smooth speed variation (realistic behavior)
    double time_sec = this->now().seconds();
    double v_left  = 0.4 + 0.1 * std::sin(time_sec);     // Varies between 0.3 ~ 0.5 m/s
    double v_right = 0.6 + 0.15 * std::sin(time_sec * 1.3); // Slightly different frequency

    double wheelbase = 0.35; // meters

    // === Forward Kinematics ===
    double linear_vel  = (v_right + v_left) / 2.0;
    double angular_vel = (v_right - v_left) / wheelbase;

    // === Update Robot Pose (Dead Reckoning) ===
    double dt = 0.05;  // 50ms = 0.05s

    theta_ += angular_vel * dt;

    // Update position using current heading
    x_ += linear_vel * std::cos(theta_) * dt;
    y_ += linear_vel * std::sin(theta_) * dt;

    // === Publish cmd_vel ===
    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.linear.x = linear_vel;
    cmd_vel.angular.z = angular_vel;
    cmd_vel_publisher_->publish(cmd_vel);

    // === Publish Odometry ===
    nav_msgs::msg::Odometry odom;
    odom.header.stamp = this->now();
    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";

    odom.pose.pose.position.x = x_;
    odom.pose.pose.position.y = y_;
    odom.pose.pose.position.z = 0.0;

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, theta_);
    odom.pose.pose.orientation.x = q.x();
    odom.pose.pose.orientation.y = q.y();
    odom.pose.pose.orientation.z = q.z();
    odom.pose.pose.orientation.w = q.w();

    odom.twist.twist.linear.x = linear_vel;
    odom.twist.twist.angular.z = angular_vel;

    odom_publisher_->publish(odom);

    // Clean logging
    RCLCPP_INFO(this->get_logger(),
      "Pose: (x=%.2f, y=%.2f, θ=%.2f) | Wheels (L=%.2f, R=%.2f) | Vel (v=%.2f, ω=%.2f)",
      x_, y_, theta_, v_left, v_right, linear_vel, angular_vel);
  }

  // Robot State (Member Variables)
  double x_ = 0.0;
  double y_ = 0.0;
  double theta_ = 0.0;

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