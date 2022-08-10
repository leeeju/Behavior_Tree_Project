
#include <memory>
#include <string>
#include <thread>
#include <iostream>
#include "get_battery_status/bt_get_battery_status.h"
#include "rclcpp/rclcpp.hpp"
#include "get_battery_status/get_battery_status_service_client.h"

//BT_REGISTER_NODES(factory) {
//  RegisterNodes(factory);
//}

using namespace std::chrono_literals;
using namespace std::placeholders;
using namespace BT;

BTGetBatteryStatus::BTGetBatteryStatus(const std::string &name, const NodeConfiguration &config)
    : ActionNodeBase(name, config) {
  get_battery_status_node_ = std::make_shared<GetBatteryStatus>();
  std::thread{std::bind(&BTGetBatteryStatus::rosSpinThread, this)}.detach();
}

void BTGetBatteryStatus::rosSpinThread() {
  rclcpp::spin(get_battery_status_node_);
}

void BTGetBatteryStatus::writeBatteryLevelOnBlackBoard(uint32_t value) {
  setOutput("battery_level", value );
}

void BTGetBatteryStatus::halt() {

}

BT::NodeStatus BTGetBatteryStatus::tick() {
//  Optional<std::uint32_t> battery_threshold;
  if (prev_status != NodeStatus::RUNNING) {
    std::cout << "send request to battery status service" << std::endl;
    get_battery_status_node_->send_get_battery_status_request(true);
    setStatus(NodeStatus::RUNNING);
    prev_status = NodeStatus::RUNNING;
  }
  if (get_battery_status_node_->getRequestStatus() == get_battery_status_node_->RUNNING) {
    std::cout << "wait for receive battery data..." << std::endl;
    prev_status = NodeStatus::RUNNING;
    return BT::NodeStatus::RUNNING;
  } else if (get_battery_status_node_->getRequestStatus() == get_battery_status_node_->FAIL) {
    std::cout << "bt get battery level failure" << std::endl;
    prev_status = NodeStatus::FAILURE;
    return BT::NodeStatus::FAILURE;
  } else if (get_battery_status_node_->getRequestStatus() == get_battery_status_node_->SUCCEED) {
    uint32_t battery_level = get_battery_status_node_->getBatteryLevel();
    std::cout << "bt get battery level success, battery level: "<< battery_level << std::endl;
    writeBatteryLevelOnBlackBoard(battery_level);
    prev_status = NodeStatus::SUCCESS;
    return BT::NodeStatus::SUCCESS;
  }
  return prev_status;
}