//
// Created by shh on 2/17/22.
//

#include <memory>
#include <string>
#include <vector>

#include "behavior_tree_engine/behavior_tree_engine.hpp"

#include <iostream>
#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/decorators/delay_node.h"
#include "get_xml_list/get_xml_list.h"

#include "unistd.h"
#include "rclcpp/rclcpp.hpp"

#include "get_battery_status/bt_get_battery_status.h"
#include "navigation_caller/bt_navigation_caller.h"
#include "check_battery/bt_check_battery.h"

#include <csignal>
#include <cstdlib>

using std::thread;
volatile sig_atomic_t flag = 1;

void check_key_interrupt(int sig) { // can be called asynchronously
  flag = 0; // set flag
  exit(0);
}

int main(int argc, char *argv[]) {
  // groot monitor port
  int zmq_publisher_port = 9999;
  int zmq_server_port = 9998;

  // bt black board init
  BT::Blackboard::Ptr blackboard_;
  blackboard_ = BT::Blackboard::create();

  // dummy vector for create bt engine instance
  const std::vector<std::string> dummy_plugin_libs = {
  };
  // create bt engine instance
  std::unique_ptr<nav2_behavior_tree::BehaviorTreeEngine> behavior_tree_engine;
  behavior_tree_engine = std::make_unique<nav2_behavior_tree::BehaviorTreeEngine>(dummy_plugin_libs);

  // reset groot monitor
  behavior_tree_engine->resetGrootMonitor();

  // ros init
  rclcpp::init(argc, argv);
  xmlListManager xmlManager;
  signal(SIGINT, check_key_interrupt);

  // read xml file
  int input = 0;
  unsigned int xml_list_amount = xmlManager.getXmlListAmount();
  std::vector<std::string> xml_list_names = xmlManager.getXmlListNames();

  // register bt node
  using namespace BT;
  behavior_tree_engine->factory_.registerNodeType<BTNavigationCaller>("NavigationCaller");
  behavior_tree_engine->factory_.registerNodeType<BTGetBatteryStatus>("GetBatteryStatus");
  behavior_tree_engine->factory_.registerNodeType<BTCheckBattery>("CheckBattery");

  NodeStatus status;

  // get selection and set bt xml file
  std::cout << "Total amount of xml files : " << xml_list_amount << std::endl;
  std::cout << "Xml file List : " << std::endl;
  for (auto const &i : xml_list_names)
    std::cout << i << std::endl;
  std::cout << "---------------------------" << std::endl;

  std::cout << "Type a number: ";

  std::cin >> input;
  auto tree = behavior_tree_engine->factory_.createTreeFromFile(xml_list_names[input - 1], blackboard_);
  behavior_tree_engine->addGrootMonitoring(&tree, zmq_publisher_port, zmq_server_port);
  while (input != -1){
    std::cout << "---------------------------" << std::endl;

    // run bt
    std::cout << "start is 1, stop is -1 " << std::endl;
    std::cout << ": ";
    std::cin >> input;

    status = NodeStatus::RUNNING;
    while (status == NodeStatus::RUNNING) {
      std::cout << "running..." << std::endl;
      status = tree.tickRoot();
      usleep(1000000);
    }
  }

  std::cout << "Adios *~" << std::endl;

  return 0;
}
