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
    // Simple velocity command (move forward while turning a little)
    geometry_msgs::msg::Twist cmd_vel;
    cmd_vel.linear.x = 0.3;   // 0.3 m/s forward
    cmd_vel.angular.z = 0.4;  // 0.4 rad/s turning left

    // Publish the command
    cmd_vel_publisher_->publish(cmd_vel);

    // Log what we are sending
    RCLCPP_INFO(this->get_logger(), 
      "Publishing cmd_vel ? linear.x = %.2f m/s, angular.z = %.2f rad/s",
      cmd_vel.linear.x, cmd_vel.angular.z);
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