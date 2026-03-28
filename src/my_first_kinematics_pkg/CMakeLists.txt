cmake_minimum_required(VERSION 3.8)
project(my_first_kinematics_pkg)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# === Eigen Setup for ROS 2 Jazzy ===
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3 REQUIRED)

find_package(geometry_msgs REQUIRED)

# Create the executable
add_executable(kinematics_calculator src/kinematics_calculator.cpp)

# Link dependencies (this adds include paths automatically)
ament_target_dependencies(kinematics_calculator
  rclcpp
  Eigen3
  geometry_msgs
)

# Install the node
install(TARGETS kinematics_calculator
  DESTINATION lib/${PROJECT_NAME})

ament_package()