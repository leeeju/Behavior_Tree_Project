#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GET_STATUS_GET_BATTERY_STATUS_SERVICE_CLIENT_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GET_STATUS_GET_BATTERY_STATUS_SERVICE_CLIENT_H_

#include <memory>
#include <string>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "bt_msgs/srv/get_battery_status.hpp"

class GetBatteryStatus : public rclcpp::Node {
 public:
  using StatusMessage = bt_msgs::srv::GetBatteryStatus;

  explicit GetBatteryStatus(
      const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());
  virtual ~GetBatteryStatus() = default;
  void send_get_battery_status_request(bool check_request);
  uint8_t getRequestStatus();
  uint8_t getBatteryLevel();
  enum RequestStatus{
    READY,
    RUNNING,
    SUCCEED,
    FAIL,
  };
 private:
  uint32_t battery_status_ = 0;
  uint8_t request_done_flag_ = READY;
  rclcpp::Client<StatusMessage>::SharedPtr get_battery_status_service_client_;
};

#endif //BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GET_STATUS_GET_BATTERY_STATUS_SERVICE_CLIENT_H_
