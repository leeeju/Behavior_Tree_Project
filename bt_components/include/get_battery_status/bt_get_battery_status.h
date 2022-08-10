#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GET_STATUS_BT_GET_BATTERY_STATUS_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_GET_STATUS_BT_GET_BATTERY_STATUS_H_

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "get_battery_status_service_client.h"

using namespace BT;

class BTGetBatteryStatus : public ActionNodeBase {
 public:
  BTGetBatteryStatus(const std::string &name, const BT::NodeConfiguration &config);
  ~BTGetBatteryStatus() override = default;

  std::shared_ptr<GetBatteryStatus> get_battery_status_node_;

  static PortsList providedPorts() {
    return {OutputPort<uint32_t>("battery_level")};
  }

  NodeStatus tick() override;
  void halt() override;
 private:
  NodeStatus prev_status = status();
  void rosSpinThread();
  void writeBatteryLevelOnBlackBoard(uint32_t value);

};

//inline void RegisterNodes(BehaviorTreeFactory &factory) {
//  factory.registerNodeType<BTGetBatteryStatus>("GetBatteryStatus");
//}

#endif 
