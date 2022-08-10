
#include <memory>
#include <chrono>
#include <cstdlib>

#include "get_battery_status/get_battery_status_service_client.h"
#include "bt_msgs/srv/get_battery_status.hpp"

GetBatteryStatus::GetBatteryStatus(const rclcpp::NodeOptions &node_options)
    : Node("get_battery_status", node_options) {
  using namespace std::chrono_literals;
  get_battery_status_service_client_ = this->create_client<StatusMessage>("battery_status_service");
  while (!get_battery_status_service_client_->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service.");
      return;
    }
    RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
  }
}
uint8_t GetBatteryStatus::getRequestStatus() {
  return request_done_flag_;
}

uint8_t GetBatteryStatus::getBatteryLevel() {
  return battery_status_;
}

void GetBatteryStatus::send_get_battery_status_request(bool check_request) {
  auto request = std::make_shared<StatusMessage::Request>();
  request_done_flag_ = RUNNING;
  request->get_battery_status = true;

  using ServiceResponseFuture = rclcpp::Client<StatusMessage>::SharedFuture;
  auto response_received_callback = [this](ServiceResponseFuture future) {
    auto response = future.get();
    battery_status_ = response->battery_status;
    RCLCPP_INFO(this->get_logger(), "Result %d \n", response->battery_status);
    request_done_flag_ = SUCCEED;
    return;
  };
  request_done_flag_ = READY;
  auto future_result = get_battery_status_service_client_->async_send_request(request, response_received_callback);
}
