#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_NAVIGATION_CALLER_BT_NAVIGATION_CALLER_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_NAVIGATION_CALLER_BT_NAVIGATION_CALLER_H_

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "navigation_caller.h"

using namespace BT;

//namespace BT {
class BTNavigationCaller : public ActionNodeBase {
 public:
  BTNavigationCaller(const std::string &name, const NodeConfiguration &config);
  ~BTNavigationCaller() override = default;

  std::shared_ptr<NavigationCaller> navigation_caller_node_;

  static PortsList providedPorts() {
    return {InputPort<std::uint32_t>("mode"),
            InputPort<std::uint32_t>("waypoint"),
            InputPort<float>("pose_x"),
            InputPort<float>("pose_y"),
            InputPort<float>("pose_z"),
            InputPort<float>("orientation_x"),
            InputPort<float>("orientation_y"),
            InputPort<float>("orientation_z"),
            InputPort<float>("orientation_w")};
  }

  NodeStatus tick() override;
  void halt() override;

 private:
  NodeStatus prev_status = status();
  void rosSpinThread();

};

//inline void RegisterNodes(BehaviorTreeFactory &factory) {
//  factory.registerNodeType<BTNavigationCaller>("NavigationCaller");
//}

#endif //BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_NAVIGATION_CALLER_BT_NAVIGATION_CALLER_H_
