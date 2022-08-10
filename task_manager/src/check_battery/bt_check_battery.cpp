

#include <memory>
#include <string>
#include <thread>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "check_battery/bt_check_battery.h"

using namespace std::chrono_literals;
using namespace std::placeholders;
using namespace BT;

BTCheckBattery::BTCheckBattery(const std::string &name, const NodeConfiguration &config)
    : ConditionNode(name, config) {
}

uint32_t BTCheckBattery::readBatteryLevel() {
  Optional<uint32_t> battery_level = getInput<uint32_t>("battery_level");
  return battery_level.value();
}

NodeStatus BTCheckBattery::tick() {
  uint32_t battery_level = readBatteryLevel();
  Optional<uint32_t> battery_threshold = getInput<uint32_t>("battery_threshold");
  if (battery_level >= battery_threshold.value()) {
    std::cout << "battery level good" << std::endl;
    return BT::NodeStatus::SUCCESS;
  } else {
    std::cout << "battery level low" << std::endl;
    return BT::NodeStatus::FAILURE;
  }
}