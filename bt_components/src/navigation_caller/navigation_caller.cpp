
#include <memory>

#include "navigation_caller/navigation_caller.h"

NavigationCaller::NavigationCaller(const rclcpp::NodeOptions &node_options)
    : Node("navigation_caller", node_options) {
  navigation_router_client_ = rclcpp_action::create_client<NavigationCallMSG>(
      this->get_node_base_interface(),
      this->get_node_graph_interface(),
      this->get_node_logging_interface(),
      this->get_node_waitables_interface(),
      "navigation_relay_action");
  using namespace std::placeholders;
  send_goal_options_.goal_response_callback =
      std::bind(&NavigationCaller::getRequestCallback, this, _1);
  send_goal_options_.feedback_callback =
      std::bind(&NavigationCaller::getFeedbackCallback, this, _1, _2);
  send_goal_options_.result_callback =
      std::bind(&NavigationCaller::getResultCallback, this, _1);
}

void NavigationCaller::requestNavigateToWayPoint(uint32_t waypoint_number) {
  request_done_flag_ = RUNNING;
  if (!this->navigation_router_client_) {
    request_done_flag_ = FAIL;
    RCLCPP_WARN(this->get_logger(), "Action Client not initialized");
  }

  if (!this->navigation_router_client_->wait_for_action_server(std::chrono::seconds(10))) {
    request_done_flag_ = FAIL;
    RCLCPP_WARN(this->get_logger(), "Action Server is not available");
    return;
  }

  goal_msg_.mode = WAYPOINT_MODE;
  goal_msg_.waypoint_number = waypoint_number;
  this->navigation_router_client_->async_send_goal(goal_msg_, send_goal_options_);
  RCLCPP_INFO(this->get_logger(), "request navigate to waypoint\n"
                                  "- waypoint number : %d", waypoint_number);
}

void NavigationCaller::requestNavigateToPose(PoseMSG pose) {
  request_done_flag_ = RUNNING;
  if (!this->navigation_router_client_) {
    request_done_flag_ = FAIL;
    RCLCPP_WARN(this->get_logger(), "Action client not initialized");
  }

  if (!this->navigation_router_client_->wait_for_action_server(std::chrono::seconds(10))) {
    request_done_flag_ = FAIL;
    RCLCPP_WARN(this->get_logger(), "Action Server is not available");
    return;
  }

  goal_msg_.mode = POSITION_MODE;
  goal_msg_.pose.pose = pose;
  this->navigation_router_client_->async_send_goal(goal_msg_, send_goal_options_);
  RCLCPP_INFO(this->get_logger(), "request navigate to pose\n"
                                  "-position x : %f \n"
                                  "-position y : %f \n"
                                  "-position z : %f \n"
                                  "-orientation x : %f \n"
                                  "-orientation y : %f \n"
                                  "-orientation z : %f \n"
                                  "-orientation w : %f \n",
              pose.position.x, pose.position.y, pose.position.z,
              pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w);
}

uint8_t NavigationCaller::getRequestStatus() {
  return request_done_flag_;
}

void NavigationCaller::getRequestCallback(
    std::shared_future<rclcpp_action::ClientGoalHandle<NavigationCallMSG>::SharedPtr> future) {
  request_done_flag_ = RUNNING;
  RCLCPP_INFO(this->get_logger(), "get response");
  auto goal_handle = future.get();
  if (!goal_handle) {
    request_done_flag_ = FAIL;
    RCLCPP_WARN(this->get_logger(), "Action goal rejected");
  } else {
    RCLCPP_INFO(this->get_logger(), "Action goal accepted");
  }
}

void NavigationCaller::getFeedbackCallback(
    NavigationCallHandle::SharedPtr,
    const std::shared_ptr<const NavigationCallMSG::Feedback> feedback) {
  RCLCPP_INFO(this->get_logger(), "Request navigate feedback\n"
                                  "current pose\n"
                                  "- position x : %f \n"
                                  "- position y : %f \n"
                                  "- position z : %f \n"
                                  "- orientation x : %f \n"
                                  "- orientation y : %f \n"
                                  "- orientation z : %f \n"
                                  "- orientation w : %f \n"
                                  "distance remaining\n"
                                  "- remain distance : %f\n"
                                  "navigation time\n"
                                  "- duration time : %d sec\n"
                                  "- duration time : %d nano sec\n",
              feedback->current_pose.pose.position.x,
              feedback->current_pose.pose.position.y,
              feedback->current_pose.pose.position.z,
              feedback->current_pose.pose.orientation.x,
              feedback->current_pose.pose.orientation.y,
              feedback->current_pose.pose.orientation.z,
              feedback->current_pose.pose.orientation.w,
              feedback->distance_remaining,
              feedback->navigation_time.sec,
              feedback->navigation_time.nanosec);
  request_done_flag_ = RUNNING;
}

void NavigationCaller::getResultCallback(const NavigationCallHandle::WrappedResult &result) {
  switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:RCLCPP_INFO(this->get_logger(), "Action succeeded!");
      request_done_flag_ = SUCCEED;
      break;
    case rclcpp_action::ResultCode::ABORTED:RCLCPP_WARN(this->get_logger(), "The action was aborted");
      request_done_flag_ = FAIL;
      return;
    case rclcpp_action::ResultCode::CANCELED:RCLCPP_WARN(this->get_logger(), "The action was canceled");
      request_done_flag_ = FAIL;
      return;
    default:RCLCPP_ERROR(this->get_logger(), "Unknown result code");
      request_done_flag_ = FAIL;
      return;
  }
//  rclcpp::shutdown();
}
