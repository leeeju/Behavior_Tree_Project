
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

BTGoToXY::BTGoToXY(const std::string &name, const NodeConfiguration &config)
: SyncActionNode(name, config){
  bt_go_to_xy = std::make_shared<GoToXY>();
  std::thread{std::bind(&BTGoToXY::rosSpinThread, this)}.detach();
  send_action_request_flag = false;
}

void BTGoToXY::rosSpinThread() {
  rclcpp::spin(bt_go_to_xy);
}

BT::NodeStatus BTGoToXY::tick() {
  if (!send_action_request_flag) {
    Optional<std::uint32_t> way_point_number = getInput<std::uint32_t>("way_point_number");
    std::cout << "get way point number: " << way_point_number.value() << std::endl;
    bt_go_to_xy->send_way_point_goal(way_point_number.value());
    send_action_request_flag = true;
  }
  std::cout << "go to xy running" << std::endl;
  while (!bt_go_to_xy->done_flag)
  {
    usleep(500000);
  }
  send_action_request_flag = false;
  return BT::NodeStatus::SUCCESS;
}

}
