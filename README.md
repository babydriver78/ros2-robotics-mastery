# ROS 2 Robotics Mastery Journey (C++ Focus)

My complete journey from absolute beginner to **master-level ROS 2 Robotics Engineer** using **ROS 2 Jazzy** and **modern C++**.  
All code is written with deep understanding, clean architecture, and industry best practices.

**Goal:** Become capable of designing, implementing, and deploying complete robotic systems (mobile robots, manipulators, perception, navigation) using ROS 2 in C++.

## Progress Log

### Phase 0: Setup & Development Environment
- [x] Installed ROS 2 Jazzy Jalisco and verified functionality
- [x] Created professional GitHub repository `ros2-robotics-mastery` with SSH
- [x] Fixed default branch to `main`
- [x] Configured VS Code with ROS extensions, IntelliSense, and `compile_commands.json`

### Phase 1: ROS 2 C++ Fundamentals & Kinematics
- [x] Created first `ament_cmake` C++ package (`my_first_kinematics_pkg`)
- [x] Understood and implemented class-based ROS 2 nodes (`public rclcpp::Node`)
- [x] Mastered `rclcpp::spin()` and continuous execution with timers
- [x] Deep understanding of `this` pointer in member functions and callbacks
- [x] Integrated Eigen3 for vector and matrix mathematics
- [x] Added `geometry_msgs` dependency and `cmd_vel` Publisher (`Twist` messages)
- [x] Implemented **Differential Drive Forward Kinematics**:
  - Linear velocity: `v = (v_r + v_l)/2`
  - Angular velocity: `ω = (v_r - v_l)/wheelbase`
  - Used `Eigen::Vector2d` for clean robot math

### Phase 2: Odometry & Pose Estimation (Current)
- [x] Added robot state as member variables (`x_`, `y_`, `theta_`)
- [x] Implemented position integration (dead reckoning) using forward kinematics
- [x] Published `nav_msgs/Odometry` with proper quaternion orientation
- [x] Used `tf2::Quaternion` for orientation conversion
- [x] Robot pose (x, y, θ) now updates continuously in real time

**Current Status:** Phase 2 in progress  
**Last Updated:** March 29, 2026

## Skills Matrix (Living Resume)

| Category                  | Skills Acquired                                              | Status     |
|---------------------------|--------------------------------------------------------------|------------|
| **ROS 2 Core**            | Nodes, Timers, Publishers, `rclcpp::spin()`, QoS             | Strong     |
| **C++**                   | Modern C++20, Classes, `this` pointer, `std::bind`, Member variables | Strong     |
| **Mathematics**           | Eigen3, Vector math, Differential Drive Kinematics, Dead Reckoning | Strong     |
| **Messages**              | `geometry_msgs/Twist`, `nav_msgs/Odometry`                   | Completed  |
| **Build System**          | ament_cmake, CMakeLists.txt, package.xml, colcon             | Strong     |
| **Development Tools**     | VS Code + ROS extension, Git, GitHub workflow                | Strong     |
| **Debugging & Logging**   | RCLCPP_INFO, IntelliSense configuration                      | Strong     |

**Next Goal (Phase 2 Completion):**  
Add variable wheel velocities (simple controller), TF broadcasting (`tf2_ros`), and prepare for Gazebo simulation.

---

**Repository Structure**
- `src/` → All ROS 2 packages
- `docs/` → Theory notes and reports (will be added later)
- Progress is tracked via Git commits and this README

**Note:** This README is updated after **every major milestone** to maintain a professional living portfolio.
