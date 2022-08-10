#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_NAVIGATION_CALLER_NAVIGATION_CALLER_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_NAVIGATION_CALLER_NAVIGATION_CALLER_H_

#include <memory>
#include <string>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "bt_msgs/action/navigation_call.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/pose.hpp"

class NavigationCaller : public rclcpp::Node {
 public:
  using NavigationCallMSG = bt_msgs::action::NavigationCall;
  using PoseMSG = geometry_msgs::msg::Pose;
  using NavigationCallHandle = rclcpp_action::ClientGoalHandle<NavigationCallMSG>;

  explicit NavigationCaller(
      const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());
  virtual ~NavigationCaller() = default;
  void requestNavigateToWayPoint(uint32_t waypoint_number);
  void requestNavigateToPose(PoseMSG pose);
  uint8_t getRequestStatus();
  enum RequestStatus{
    READY,
    RUNNING,
    SUCCEED,
    FAIL,
  };

  enum Mode{
    UNKNOWN_MODE,
    WAYPOINT_MODE,
    POSITION_MODE,
  };

 private:
  uint8_t request_done_flag_ = READY;
  NavigationCallMSG::Goal goal_msg_ = NavigationCallMSG::Goal();
  rclcpp_action::Client<NavigationCallMSG>::SendGoalOptions send_goal_options_ = rclcpp_action::Client<NavigationCallMSG>::SendGoalOptions();
  void getRequestCallback(
      std::shared_future<rclcpp_action::ClientGoalHandle<NavigationCallMSG>::SharedPtr> future);
  void getFeedbackCallback(
      NavigationCallHandle::SharedPtr,
      const std::shared_ptr<const NavigationCallMSG::Feedback> feedback);
  void getResultCallback(
      const NavigationCallHandle::WrappedResult &result);
  rclcpp_action::Client<NavigationCallMSG>::SharedPtr navigation_router_client_;
};

#endif //BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_NAVIGATION_CALLER_NAVIGATION_CALLER_H_
