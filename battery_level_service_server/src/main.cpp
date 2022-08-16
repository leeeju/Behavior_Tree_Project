
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <thread>
#include "unistd.h"

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"

#include "battery_level_service_server.h"

void print_help() {
  printf("For Node node:\n");
  printf("node_name [-h]\n");
  printf("Options:\n");
  printf("\t-h Help           : Print this help function.\n");
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<BatteryLevelServer>();
  std::cout << "run battery level service server " << std::endl;
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
