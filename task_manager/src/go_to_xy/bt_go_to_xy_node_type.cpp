
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <thread>
#include "unistd.h"
#include <iostream>
#include "go_to_xy/bt_go_to_xy.h"
#include "rclcpp/rclcpp.hpp"
#include "go_to_xy/go_to_xy_action_client.h"
#include <thread>

BT_REGISTER_NODES(factory) {
  BehaviorTreeNodeGoToXY::RegisterNodes(factory);
}

using namespace std::chrono_literals;
using namespace std::placeholders;
using namespace BT;

namespace BehaviorTreeNodeGoToXY {

BTGoToXY::BTGoToXY() {
//  int argc = 0;
//  char *argv[1];
//  rclcpp::init(argc, argv);
//  bt_go_to_xy = std::make_shared<GoToXY>();
//  rclcpp::spin(bt_go_to_xy);
  std::thread{std::bind(&BTGoToXY::rosSpinThread, this)}.detach();
  send_action_request_flag = false;
}

void BTGoToXY::rosSpinThread() {
//  int argc = 0;
//  char *argv[1];
//  rclcpp::init(argc, argv);
  bt_go_to_xy = std::make_shared<GoToXY>();
  rclcpp::spin(bt_go_to_xy);
}

BT::NodeStatus BTGoToXY::requestGoToXY(BT::TreeNode& self) {
  Optional<std::uint32_t> way_point_number = self.getInput<std::uint32_t>("way_point_number");
  std::cout << "get way point number: " << way_point_number.value() << std::endl;
  if (!send_action_request_flag) {
//    bt_go_to_xy->send_way_point_goal(way_point_number.value());
    send_action_request_flag = true;
  }

  if (bt_go_to_xy->done_flag) {
    send_action_request_flag = false;
    return BT::NodeStatus::SUCCESS;
  } else {
    return BT::NodeStatus::RUNNING;
  }
}

}


