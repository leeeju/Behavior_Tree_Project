
#include <memory>

#include "go_to_xy/go_to_xy_action_client.h"

GoToXY::GoToXY(const rclcpp::NodeOptions &node_options)
    : Node("bt_go_to_xy", node_options) {
  go_to_xy_action_client_ = rclcpp_action::create_client<GoalCommander>(
      this->get_node_base_interface(),
      this->get_node_graph_interface(),
      this->get_node_logging_interface(),
      this->get_node_waitables_interface(),
      "go_to_xy_action_server");
  using namespace std::placeholders;
  goal_msg_ = GoalCommander::Goal();
  send_goal_options_ = rclcpp_action::Client<GoalCommander>::SendGoalOptions();
  send_goal_options_.goal_response_callback =
      std::bind(&GoToXY::get_go_to_xy_action_response_callback, this , _1);
  send_goal_options_.feedback_callback =
      std::bind(&GoToXY::get_go_to_xy_action_feedback, this, _1, _2);
  send_goal_options_.result_callback =
      std::bind(&GoToXY::get_go_to_xy_action_result, this, _1);
}

void GoToXY::send_way_point_goal(uint32_t way_point_number) {
  done_flag = false;

  if (!this->go_to_xy_action_client_) {
    RCLCPP_WARN(this->get_logger(), "Action client not initialized");
  }

  if (!this->go_to_xy_action_client_->wait_for_action_server(std::chrono::seconds(10))) {
    RCLCPP_WARN(this->get_logger(), "Test action server is not available.");
    return;
  }

  goal_msg_.way_point_number = way_point_number;
  this->go_to_xy_action_client_->async_send_goal(goal_msg_, send_goal_options_);
  RCLCPP_INFO(this->get_logger(), "send way point goal: %d", way_point_number);
}

void GoToXY::get_go_to_xy_action_response_callback(
    std::shared_future<GoalHandleCommander::SharedPtr> future) {
  done_flag = false;
  RCLCPP_INFO(this->get_logger(), "get response");
  auto goal_handle = future.get();
  if (!goal_handle) {
    done_flag = true;
    RCLCPP_WARN(this->get_logger(), "Action goal rejected.");
  } else {
    done_flag = false;
    RCLCPP_INFO(this->get_logger(), "Action goal accepted.");
  }
}

void GoToXY::get_go_to_xy_action_feedback(
    GoalHandleCommander::SharedPtr,
    const std::shared_ptr<const GoalCommander::Feedback> feedback) {
  RCLCPP_INFO(this->get_logger(), "Action feedback: %d", feedback->go_to_xy_feedback);
  done_flag = false;
}

void GoToXY::get_go_to_xy_action_result(
    const GoalHandleCommander::WrappedResult &result) {
  switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:RCLCPP_INFO(this->get_logger(), "Action succeeded!");
      done_flag = true;
      break;
    case rclcpp_action::ResultCode::ABORTED:RCLCPP_WARN(this->get_logger(), "The action was aborted");
      done_flag = true;
      return;
    case rclcpp_action::ResultCode::CANCELED:RCLCPP_WARN(this->get_logger(), "The action was canceled");
      done_flag = true;
      return;
    default:RCLCPP_ERROR(this->get_logger(), "Unknown result code");
      done_flag = true;
      return;
  }
  done_flag = true;
//  rclcpp::shutdown();
}
