//
// Created by shh on 2/14/22.
//

#ifndef BT_PREVENTION_MODULE_SRC_BATTERY_LEVEL_SERVICE_SERVER_INCLUDE_BATTERY_LEVEL_SERVICE_SERVER_H_
#define BT_PREVENTION_MODULE_SRC_BATTERY_LEVEL_SERVICE_SERVER_INCLUDE_BATTERY_LEVEL_SERVICE_SERVER_H_

#include <memory>
#include <string>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "bt_msgs/srv/get_battery_status.hpp"

class BatteryLevelServer : public rclcpp::Node {
 public:
  using BatteryMSG = bt_msgs::srv::GetBatteryStatus;

  explicit BatteryLevelServer(
      const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());
  virtual ~BatteryLevelServer() = default;

 private:
  void returnBatteryLevelCallback(const std::shared_ptr<BatteryMSG::Request> request,
                                  const std::shared_ptr<BatteryMSG::Response> response);
  void getBatteryLevelCallback(const std_msgs::msg::UInt32::SharedPtr msg);
  uint32_t battery_level_ = 0;
//  std::shared_ptr<rclcpp::Node> node_;
  rclcpp::Service<BatteryMSG>::SharedPtr service_server_;
  rclcpp::Subscription<std_msgs::msg::UInt32>::SharedPtr battery_level_subscriber_;
};
#endif //BT_PREVENTION_MODULE_SRC_BATTERY_LEVEL_SERVICE_SERVER_INCLUDE_BATTERY_LEVEL_SERVICE_SERVER_H_
