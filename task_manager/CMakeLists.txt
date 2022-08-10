cmake_minimum_required(VERSION 3.5)
project(task_manager)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()
include_directories(include /usr/local/include/behaviortree_cpp_v3/ /usr/local/include/ /opt/ros/foxy/include/)
add_compile_definitions(_XML_LIST_PATH_="${PROJECT_SOURCE_DIR}/src/xml")
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(bt_msgs REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

#add_executable(bt_executor
#        src/main.cpp
#        src/go_to_xy/bt_go_to_xy.cpp
#        src/go_to_xy/go_to_xy_action_client.cpp
#        src/get_battery_status/bt_get_battery_status.cpp
#        src/get_battery_status/get_battery_status_service_client.cpp
#        src/check_battery/bt_check_battery.cpp
#        src/navigation_caller/bt_navigation_caller.cpp
#        src/navigation_caller/navigation_caller.cpp
#        include/get_xml_list/get_xml_list.cpp
#        )
#ament_target_dependencies(bt_executor
#        bt_msgs
#        rclcpp
#        rclcpp_action
#        behaviortree_cpp_v3
#        )

add_executable(task_executor
        src/behavior_tree_engine/main.cpp
        src/behavior_tree_engine/behavior_tree_engine.cpp
        src/go_to_xy/bt_go_to_xy.cpp
        src/go_to_xy/go_to_xy_action_client.cpp
        src/get_battery_status/bt_get_battery_status.cpp
        src/get_battery_status/get_battery_status_service_client.cpp
        src/check_battery/bt_check_battery.cpp
        src/navigation_caller/bt_navigation_caller.cpp
        src/navigation_caller/navigation_caller.cpp
        include/get_xml_list/get_xml_list.cpp
        )
ament_target_dependencies(task_executor
        bt_msgs
        rclcpp
        rclcpp_action
        behaviortree_cpp_v3
        )

#add_executable(get_battery_status
#        src/get_battery_status/get_battery_status_main.cpp
#        src/get_battery_status/get_battery_status_service_client.cpp
#        )
#ament_target_dependencies(get_battery_status
#        bt_msgs
#        rclcpp
#        rclcpp_action
#        behaviortree_cpp_v3
#        )
#
#add_executable(navigation_caller
#        src/navigation_caller/navigation_caller_main.cpp
#        src/navigation_caller/navigation_caller.cpp
#        )
#ament_target_dependencies(navigation_caller
#        bt_msgs
#        rclcpp
#        rclcpp_action
#        behaviortree_cpp_v3
#        )

install(TARGETS
        task_executor
#        navigation_caller
#        get_battery_status
#        bt_executor
        DESTINATION lib/${PROJECT_NAME})

ament_package()