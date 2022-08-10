from rclpy.qos import *
from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.action import ActionServer
from nav2_msgs.action import NavigateToPose

from geometry_msgs.msg import PoseStamped

from action_msgs.msg import GoalStatus
from bt_msgs.action import NavigationCall

from rclpy.callback_groups import ReentrantCallbackGroup

import pathlib
import yaml
import rclpy
import threading
import time
import math


class NavigationRelayServer(Node, threading.Thread):
    def __init__(self):
        rclpy.init()
        super().__init__('navigation_relay_server')
        threading.Thread.__init__(self)
        """setting file path"""
        self.__module_path = str(pathlib.Path(__file__).parent.absolute())
        self.__point_file_path = self.__module_path + "/point_list/waypoint_list.yaml"

        self.setDaemon(True)
        # self.__is_alive = False
        # self.__qos_profile = QoSProfile(reliability=ReliabilityPolicy.RMW_QOS_POLICY_RELIABILITY_RELIABLE,
        #                                 history=HistoryPolicy.RMW_QOS_POLICY_HISTORY_KEEP_LAST,
        #                                 depth=10,
        #                                 durability=DurabilityPolicy.RMW_QOS_POLICY_DURABILITY_VOLATILE)

        self.__node_instance = None
        self.callback_group = ReentrantCallbackGroup()

        self.action_client = ActionClient(self, action_type=NavigateToPose, action_name='/navigate_to_pose')
        self.action_goal_handle = None
        self.action_result_future = None
        self.action_feedback = None
        self.action_status = None
        self.feedback_msg = NavigationCall.Feedback()

        self.declare_parameter('/controller_server FollowPath.max_vel_x')

        self.param_move_speed = rclpy.parameter.Parameter('/controller_server FollowPath.max_vel_x',
                                                          rclpy.Parameter.Type.DOUBLE,
                                                          0.5)
        self.param_prevention_speed = rclpy.parameter.Parameter('/controller_server FollowPath.max_vel_x',
                                                                rclpy.Parameter.Type.DOUBLE,
                                                                0.1)
        # self.action_server = ActionServer(node=self,
        #                                   action_type=NavigationCall,
        #                                   action_name='navigation_relay_action',
        #                                   execute_callback=self.execute_navigation_relay_action,
        #                                   goal_service_qos_profile=self.__qos_profile)
        self.action_server = ActionServer(node=self,
                                          action_type=NavigationCall,
                                          action_name='navigation_relay_action',
                                          execute_callback=self.execute_navigation_relay_action)

        self.__INDEX_POSE = 0
        self.__INDEX_ORIENTATION = 1
        self.__INDEX_X = 0
        self.__INDEX_Y = 1
        self.__INDEX_Z = 2
        self.__INDEX_W = 3

        self.__point_list = []

        self.__read_way_point_yaml()

    def __del__(self):
        self.__is_alive = False
        self.destroy_node()
        rclpy.shutdown()

    def __read_way_point_yaml(self):
        with open(self.__point_file_path) as file:
            waypoint_data = yaml.safe_load(file)
        for waypoint_number, point in waypoint_data.items():
            position_data_x = None
            position_data_y = None
            position_data_z = None
            position_data_w = None
            pose = [None, None, None]
            orientation = [None, None, None, None]
            for point_tag, value_tag in point.items():
                for tag, value in value_tag.items():
                    if tag == 'x':
                        position_data_x = value
                    elif tag == 'y':
                        position_data_y = value
                    elif tag == 'z':
                        position_data_z = value
                    elif tag == 'w':
                        position_data_w = value
                if point_tag == "goal_pose":
                    pose = [position_data_x, position_data_y, position_data_z]
                elif point_tag == "orientation":
                    orientation = [position_data_x, position_data_y, position_data_z, position_data_w]
            waypoint = [pose, orientation]
            self.__point_list.append(waypoint)

    def execute_navigation_relay_action(self, goal_handle):
        self.get_logger().info('Execute navigation relay server action!')
        if goal_handle.request.mode == 1:
            self.get_logger().info('selected waypoint mode')
            position = self.__point_list[goal_handle.request.waypoint_number][self.__INDEX_POSE]
            orientation = self.__point_list[goal_handle.request.waypoint_number][self.__INDEX_ORIENTATION]
            self.go_to_goal(position, orientation)

        elif goal_handle.request.mode == 2:
            self.get_logger().info('selected pose mode')
            position = [goal_handle.request.pose.pose.position.x,
                        goal_handle.request.pose.pose.position.y,
                        goal_handle.request.pose.pose.position.z]
            orientation = [goal_handle.request.pose.pose.orientation.x,
                           goal_handle.request.pose.pose.orientation.y,
                           goal_handle.request.pose.pose.orientation.z,
                           goal_handle.request.pose.pose.orientation.w]
            self.go_to_goal(position, orientation)

        while not self.is_move_to_goal_complete():
            print("move to point...")
            goal_handle.publish_feedback(self.feedback_msg)
            time.sleep(1)
        print("move done")
        goal_handle.succeed()
        result = NavigationCall.Result()
        print("return succeed")
        return result

    def set_speed_mode_move(self):
        param = [self.param_move_speed]
        self.set_parameters(param)

    def set_speed_mode_prevention(self):
        param = [self.param_prevention_speed]
        self.set_parameters(param)

    def go_to_goal(self, goal_pose, orientation):
        print("go to goal")
        goal_msg = NavigateToPose.Goal()
        to_pose_msg = PoseStamped()
        to_pose_msg.pose.position.x = goal_pose[0]
        to_pose_msg.pose.position.y = goal_pose[1]
        to_pose_msg.pose.position.z = goal_pose[2]
        to_pose_msg.pose.orientation.x = orientation[0]
        to_pose_msg.pose.orientation.y = orientation[1]
        to_pose_msg.pose.orientation.z = orientation[2]
        to_pose_msg.pose.orientation.w = orientation[3]
        goal_msg.pose = to_pose_msg

        future = self.action_client.send_goal_async(goal_msg, self.__navi_action_feedback_callback)
        time.sleep(2)
        print("spin until complete")
        rclpy.spin_until_future_complete(node=self, future=future)
        print("spin done")
        self.action_goal_handle = future.result()
        if self.action_goal_handle is not None:
            if not self.action_goal_handle.accepted:
                print('go to goal was rejected!')
                return False
        elif self.action_goal_handle is None:
            return False
        self.action_result_future = self.action_goal_handle.get_result_async()
        return True

    def cancel_move_to_goal(self):
        try:
            print('Canceling current goal.')
            if self.action_result_future:
                future = self.action_goal_handle.cancel_goal_async()
                rclpy.spin_until_future_complete(node=self, future=future)
            return True
        except Exception as e:
            print(e)
            return False

    def is_move_to_goal_complete(self):
        if not self.action_result_future:
            # task was cancelled or completed
            return True
        rclpy.spin_until_future_complete(node=self, future=self.action_result_future)
        if self.action_result_future.result():
            self.action_status = self.action_result_future.result().status
            if self.action_status != GoalStatus.STATUS_SUCCEEDED:
                print('Goal with failed with status code: {0}'.format(self.action_status))
                return True
        else:
            # Timed out, still processing, not complete yet
            return False

        print('Goal succeeded!')
        return True

    def __navi_action_feedback_callback(self, msg):
        self.feedback = msg.feedback
        return

    def get_go_to_goal_result(self):
        return self.action_status

    def run(self) -> None:
        self.node_main()
        # self.__is_alive = self.is_alive()
        # while self.__is_alive:

    @staticmethod
    def calculate_orientation(x_1, y_1, x_2, y_2):
        y = y_2 - y_1
        x = x_2 - x_1
        degree = math.atan2(y, x) * 180 / 3.1415
        orientation_z = math.sin(degree / 2)
        orientation_w = math.cos(degree / 2)
        return 0.0, 0.0, orientation_z, orientation_w

    def node_main(self):
        while True:
            time.sleep(1)
        # rclpy.spin(self)

    def ros_spin_once(self):
        rclpy.spin_once(self, timeout_sec=0.1)


if __name__ == '__main__':
    node_instance = NavigationRelayServer()
    node_instance.start()
    print("run!")
    while True:
        node_instance.ros_spin_once()
        time.sleep(1)
