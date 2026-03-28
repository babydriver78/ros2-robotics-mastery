#include <rclcpp/rclcpp.hpp>        // Core ROS 2 C++ library (nodes, logging, timers, etc.)
#include <chrono>                   // For time literals like 500ms

using namespace std::chrono_literals;   // Allows us to write 500ms instead of std::chrono::milliseconds(500)

// This is the modern, professional way to create a ROS 2 node
class KinematicsCalculator : public rclcpp::Node
{
public:
  // Constructor - runs when the node is created
  KinematicsCalculator() : Node("kinematics_calculator")
  {
    // Log message (visible with ros2 run or in rqt_console)
    RCLCPP_INFO(this->get_logger(), "Kinematics Calculator Node started successfully!");

    // Create a wall timer (real-time timer, not affected by simulation time yet)
    // This timer will call timer_callback() every 500 milliseconds
    timer_ = this->create_wall_timer(
      500ms,                                      // Period
      std::bind(&KinematicsCalculator::timer_callback, this));  // What to call
  }

private:
  // This function runs repeatedly - this is where your robot logic goes
  void timer_callback()
  {
    static int counter = 0;                       // static = remembers value between calls
    RCLCPP_INFO(this->get_logger(), 
                "Kinematics update #%d - Node is alive and ready for robot math!", 
                ++counter);
  }

  // Member variable to store the timer
  rclcpp::TimerBase::SharedPtr timer_;
};

// Main function - entry point of the executable
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);     // Initialize ROS 2 communication layer (DDS)

  // Create the node object and give control to ROS 2 event loop
  rclcpp::spin(std::make_shared<KinematicsCalculator>());

  rclcpp::shutdown();           // Clean shutdown when Ctrl+C is pressed
  return 0;
}