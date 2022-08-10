//
// Created by shh on 2/11/22.
//
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <thread>
#include "unistd.h"
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "navigation_caller/bt_navigation_caller.h"
#include "navigation_caller/navigation_caller.h"
#include <thread>

using namespace std::chrono_literals;
using namespace std::placeholders;
using namespace BT;

//BT_REGISTER_NODES(factory) {
//  RegisterNodes(factory);
//}

BTNavigationCaller::BTNavigationCaller(const std::string &name, const NodeConfiguration &config)
    : ActionNodeBase(name, config) {
  navigation_caller_node_ = std::make_shared<NavigationCaller>();
  std::thread{std::bind(&BTNavigationCaller::rosSpinThread, this)}.detach();
}

void BTNavigationCaller::rosSpinThread() {
  rclcpp::spin(navigation_caller_node_);
}

void BTNavigationCaller::halt() {

}

NodeStatus BTNavigationCaller::tick() {
  if (prev_status != NodeStatus::RUNNING) {
    std::cout << "bt tick " << std::endl;
    Optional<std::uint32_t> mode = getInput<std::uint32_t>("mode");
    if (mode.value() == navigation_caller_node_->WAYPOINT_MODE) {
      Optional<std::uint32_t> waypoint_number = getInput<std::uint32_t>("waypoint");
      std::cout << "get way point number: " << waypoint_number.value() << std::endl;
      navigation_caller_node_->requestNavigateToWayPoint(waypoint_number.value());
    } else if (mode.value() == navigation_caller_node_->POSITION_MODE) {
      Optional<float> pose_x = getInput<float>("pose_x");
      Optional<float> pose_y = getInput<float>("pose_y");
      Optional<float> pose_z = getInput<float>("pose_z");
      Optional<float> orientation_x = getInput<float>("orientation_x");
      Optional<float> orientation_y = getInput<float>("orientation_y");
      Optional<float> orientation_z = getInput<float>("orientation_z");
      Optional<float> orientation_w = getInput<float>("orientation_w");
      geometry_msgs::msg::Pose point;
      point.position.x = pose_x.value();
      point.position.y = pose_y.value();
      point.position.z = pose_z.value();
      point.orientation.x = orientation_x.value();
      point.orientation.y = orientation_y.value();
      point.orientation.z = orientation_z.value();
      point.orientation.w = orientation_w.value();
      std::cout << "get position " << std::endl;
      navigation_caller_node_->requestNavigateToPose(point);
    }
    setStatus(NodeStatus::RUNNING);
    prev_status = NodeStatus::RUNNING;
  }
  if (navigation_caller_node_->getRequestStatus() == navigation_caller_node_->RUNNING) {
    std::cout << "running bt navigation caller " << std::endl;
    prev_status = NodeStatus::RUNNING;
    return BT::NodeStatus::RUNNING;
  } else if (navigation_caller_node_->getRequestStatus() == navigation_caller_node_->FAIL) {
    std::cout << "bt navigation caller failure" << std::endl;
    prev_status = NodeStatus::FAILURE;
    return BT::NodeStatus::FAILURE;
  } else if (navigation_caller_node_->getRequestStatus() == navigation_caller_node_->SUCCEED) {
    std::cout << "bt navigation caller success " << std::endl;
    prev_status = NodeStatus::SUCCESS;
    return BT::NodeStatus::SUCCESS;
  }
  return prev_status;
}
