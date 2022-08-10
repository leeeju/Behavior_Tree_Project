//
// Created by shh on 2/4/22.
//

#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_BT_GO_TO_XY_EX_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_BT_GO_TO_XY_EX_H_

#include <memory>
#include <string>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "bt_msgs/action/go_to_xy_action.hpp"

class GoToXY : public rclcpp::Node {
 public:
  using GoalCommander = bt_msgs::action::GoToXYAction;
  using GoalHandleCommander = rclcpp_action::ClientGoalHandle<GoalCommander>;

  explicit GoToXY(
      const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());
  virtual ~GoToXY() = default;
  void send_way_point_goal(uint32_t way_point_number);
  bool done_flag = false;

 private:

  GoalCommander::Goal goal_msg_;
  rclcpp_action::Client<GoalCommander>::SendGoalOptions send_goal_options_;
  void get_go_to_xy_action_response_callback(
      std::shared_future<rclcpp_action::ClientGoalHandle<GoalCommander>::SharedPtr> future);

  void get_go_to_xy_action_feedback(
      GoalHandleCommander::SharedPtr,
      const std::shared_ptr<const GoalCommander::Feedback> feedback);

  void get_go_to_xy_action_result(
      const GoalHandleCommander::WrappedResult &result);

  rclcpp_action::Client<GoalCommander>::SharedPtr go_to_xy_action_client_;
};

#endif //BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_BT_GO_TO_XY_EX_H_
