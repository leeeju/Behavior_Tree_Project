//
// Created by shh on 2/12/22.
//

#ifndef BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_CHECK_BATTERY_BT_CHECK_BATTERY_H_
#define BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_CHECK_BATTERY_BT_CHECK_BATTERY_H_

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"

using namespace BT;

class BTCheckBattery : public ConditionNode {
 public:
  BTCheckBattery(const std::string &name, const BT::NodeConfiguration &config);
  ~BTCheckBattery() override = default;

  static PortsList providedPorts() {
    return {InputPort<uint32_t>("battery_level"),
            InputPort<uint32_t>("battery_threshold")};
  }

  NodeStatus tick() override;
 private:
  uint32_t readBatteryLevel();

};

#endif //BT_PREVENTION_MODULE_SRC_BT_PREVENTION_MODULE_INCLUDE_CHECK_BATTERY_BT_CHECK_BATTERY_H_
