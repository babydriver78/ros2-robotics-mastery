#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <Eigen/Dense>
#include <geometry_msgs/msg/twist.hpp>

using namespace std::chrono_literals;

class SimpleKinematicsNode : public rclcpp::Node
{
public:
  SimpleKinematicsNode() : Node("simple_kinematics_node")
  {
    RCLCPP_INFO(this->get_logger(), "? Simple Kinematics Node started!");

    // === NEW: Create a Publisher for cmd_vel ===
    // This will publish velocity commands that a robot (or simulator) can listen to
    cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

    // Timer at 10 Hz (common control rate)
    timer_ = this->create_wall_timer(
      1000ms,
      std::bind(&SimpleKinematicsNode::control_loop, this));
  }

private:
  void control_loop()
  {
    double v_left = 0.5;  // Left wheel velocity (m/s)
    double v_right = 0.7; // Right wheel velocity (m/s)
    double wheel_base = 0.35; // Distance between wheels (m)
    // Compute linear and angular velocity
    double linear_vel = (v_left + v_right) / 2.0;
    double angular_vel = (v_right - v_left) / wheel_base;
    // Create a Twist message to publish
    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.linear.x = linear_vel; // Forward velocity
    cmd_vel.angular.z = angular_vel; // Rotation around Z-axis
    // Publish the velocity command
    cmd_vel_publisher_->publish(cmd_vel);

    //Log the computed velocities for debugging
    RCLCPP_INFO(this->get_logger(),
      "Forward Kinematics → Wheels (L:%.2f, R:%.2f) | Robot (v=%.2f m/s, ω=%.2f rad/s)",
      v_left, v_right, linear_vel, angular_vel); 
  }

  // === NEW MEMBER: Publisher ===
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;

  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SimpleKinematicsNode>());
  rclcpp::shutdown();
  return 0;
}