
#include <memory>
#include <chrono>

#include "battery_level_service_server.h"

BatteryLevelServer::BatteryLevelServer(const rclcpp::NodeOptions &node_options)
    : Node("battery_level_service_server", node_options) {
  using namespace std::placeholders;
  service_server_ = this->create_service<BatteryMSG>("battery_status_service",
                                                     std::bind(&BatteryLevelServer::returnBatteryLevelCallback, this,
                                                               _1, _2));
  battery_level_subscriber_ = this->create_subscription<std_msgs::msg::UInt32>
      ("battery_data",
       10,
       std::bind(&BatteryLevelServer::getBatteryLevelCallback, this, _1));
}

void BatteryLevelServer::returnBatteryLevelCallback(const std::shared_ptr<BatteryMSG::Request> request,
                                                    const std::shared_ptr<BatteryMSG::Response> response) {
  if (request->get_battery_status) {
    response->battery_status = battery_level_;
  }
}

void BatteryLevelServer::getBatteryLevelCallback(const std_msgs::msg::UInt32::SharedPtr msg) {
  battery_level_ = msg->data;
  RCLCPP_INFO(this->get_logger(), "received battery level: %d", battery_level_);
}

