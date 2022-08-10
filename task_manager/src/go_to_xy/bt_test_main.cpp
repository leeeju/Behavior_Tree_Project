// Copyright 2021 OROCA
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <thread>
#include "unistd.h"

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"

#include "go_to_xy/go_to_xy_action_client.h"

using std::thread;

void print_help()
{
  printf("For Node node:\n");
  printf("node_name [-h]\n");
  printf("Options:\n");
  printf("\t-h Help           : Print this help function.\n");
}

int main(int argc, char * argv[])
{
//  if (rcutils_cli_option_exist(argv, argv + argc, "-h")) {
//    print_help();
//    return 0;
//  }
  int input = 0;

  rclcpp::init(argc, argv);
  auto bt_go_to_xy = std::make_shared<GoToXY>();
  auto spin_executor = [&bt_go_to_xy]() {
    rclcpp::spin(bt_go_to_xy);
  };
  thread t1(spin_executor);

  while (input != -1){
    std::cout << "Type a number: ";

    std::cin >> input;
    if (input != 0 ){

      bt_go_to_xy->send_way_point_goal(1);
      input = 1;
    }
    if (input == 1 ){
      std::cout << "action is running";
      usleep(10000);
      if (bt_go_to_xy->done_flag){
        input = 0;
      }
    }
  }

//  rclcpp::spin(bt_go_to_xy);
  rclcpp::shutdown();

  return 0;
}