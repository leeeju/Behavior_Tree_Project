
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <thread>
#include "unistd.h"

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"

#include "navigation_caller/navigation_caller.h"
#include "geometry_msgs/msg/pose.hpp"

using std::thread;

void print_help() {
  printf("For Node node:\n");
  printf("node_name [-h]\n");
  printf("Options:\n");
  printf("\t-h Help           : Print this help function.\n");
}

int main(int argc, char *argv[]) {
//  if (rcutils_cli_option_exist(argv, argv + argc, "-h")) {
//    print_help();
//    return 0;
//  }
  int input = 0;

  rclcpp::init(argc, argv);
  auto test_node = std::make_shared<NavigationCaller>();
  auto spin_executor = [&test_node]() {
    rclcpp::spin(test_node);
  };
  thread t1(spin_executor);

  while (input != -1) {
    std::cout << "Type a number: " << std::endl;

    std::cin >> input;

    if (input == 1) {
      test_node->requestNavigateToWayPoint(3);
      while (test_node->getRequestStatus() == test_node->RUNNING) {
        std::cout << "navigate to waypoint running" << std::endl;
        usleep(500000);
      }
      if (test_node->getRequestStatus() == test_node->READY) {
        std::cout << "action is READY STATUS.. is possible?" << std::endl;
      } else if (test_node->getRequestStatus() == test_node->RUNNING) {
        std::cout << "action is RUNNING STATUS.. is possible?" << std::endl;
      } else if (test_node->getRequestStatus() == test_node->SUCCEED) {
        std::cout << "action is SUCCEED STATUS. Good!" << std::endl;
      } else if (test_node->getRequestStatus() == test_node->FAIL) {
        std::cout << "action is FAIL STATUS. Sad..." << std::endl;
      }
    } else if (input == 2) {
      geometry_msgs::msg::Pose test_pose_msg;
      test_pose_msg.position.x = 0.04943786561489105;
      test_pose_msg.position.y = -1.9803409576416016;
      test_pose_msg.position.z = 0.0;
      test_pose_msg.orientation.x = 0.0;
      test_pose_msg.orientation.y = 0.0;
      test_pose_msg.orientation.z = 0.10454357591217914;
      test_pose_msg.orientation.w = 0.994520306849234;
      test_node->requestNavigateToPose(test_pose_msg);
      while (test_node->getRequestStatus() == test_node->RUNNING) {
        std::cout << "navigate to pose running" << std::endl;
        usleep(500000);
      }
      if (test_node->getRequestStatus() == test_node->READY) {
        std::cout << "action is READY STATUS.. is possible?"<< std::endl;
      } else if (test_node->getRequestStatus() == test_node->RUNNING) {
        std::cout << "action is RUNNING STATUS.. is possible?"<< std::endl;
      } else if (test_node->getRequestStatus() == test_node->SUCCEED) {
        std::cout << "action is SUCCEED STATUS. Good!"<< std::endl;
      } else if (test_node->getRequestStatus() == test_node->FAIL) {
        std::cout << "action is FAIL STATUS. Sad..."<< std::endl;
      }
    }
    input = 0;
  }

//  rclcpp::spin(test_node);
  rclcpp::shutdown();

  return 0;
}