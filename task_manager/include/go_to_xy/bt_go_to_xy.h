//
// Created by shh on 2/6/22.
//

#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GO_TO_XY_BT_GO_TO_XY_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GO_TO_XY_BT_GO_TO_XY_H_

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "go_to_xy_action_client.h"

//using namespace BT;

namespace BehaviorTreeNodeGoToXY {
class BTGoToXY : public BT::SyncActionNode {
 public:
  BTGoToXY(const std::string &name, const BT::NodeConfiguration &config);
//  ~BTGoToXY() = default;
  std::shared_ptr<GoToXY> bt_go_to_xy;
//  BT::NodeStatus ActionGoToXY();

  static BT::PortsList providedPorts()
  {
    // This action has a single input port called "message"
    // Any port must have a name. The type is optional.
    return { BT::InputPort<std::uint32_t>("way_point_number") };
  }

  BT::NodeStatus tick() override;
 private:
  void rosSpinThread();
  bool send_action_request_flag;

};

inline void RegisterNodes(BT::BehaviorTreeFactory &factory) {
  factory.registerNodeType<BTGoToXY>("GoToXY");
}

}
#endif //BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GO_TO_XY_BT_GO_TO_XY_H_

