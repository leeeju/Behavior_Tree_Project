cmake_minimum_required(VERSION 3.5)
project(battery_level_service_server)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include /usr/local/include/behaviortree_cpp_v3/ /usr/local/include/ /opt/ros/foxy/include/)
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(bt_msgs REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)

add_executable(battery_level_service_server
        src/main.cpp
        src/battery_level_service_server.cpp
        )
ament_target_dependencies(battery_level_service_server
        bt_msgs
        rclcpp
        std_msgs
        )

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
        battery_level_service_server
        DESTINATION lib/${PROJECT_NAME})

ament_package()
