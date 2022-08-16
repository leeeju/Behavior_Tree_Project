# Behavior Tree Demo

## Requirements
- OS: Ubuntu20.04
- ROS2: Foxy

## How to build and install

1. navigation2 패키지를 설치합니다.
   <pre><code>sudo apt install ros-foxy-navigation2 ros-foxy-nav2-bringup '~ros-foxy-turtlebot3-.*'</code></pre>
2. navigation_relay_server 패키지에서 사용할 python 패키지를 설치합니다.
   <pre><code>cd ~/behavior_ws/src/navigation_relay_server/
   pip install -r requirements.txt</code></pre>
3. 해당 워크 스페이스를 빌드합니다.
   <pre><code>cd ~/behavior_ws
   colcon build --packages-select bt_msgs --symlink-install
   colcon build --symlink-install </code></pre>

## How to run
1. 해당 워크 스페이스를 source 합니다.
   <pre><code>source ~/behavior_ws/install/setup.bash</code></pre>
2. navigation2 시뮬레이션과 연동하여 테스트 하기 위하여 turtlebot3 시뮬레이션을 실행합니다.
   <pre><code>ros2 launch nav2_bringup tb3_simulation_launch.py</code></pre>
3. navigation 과 ros 통신하여 주행 요청을 하는 navigation_relay_server를 실행합니다.
   <pre><code>ros2 run navigation_relay_server navigation_relay_server </code></pre>
4. battery 토픽을 subscribe해서 service로 반환하는 battery_level_service_server를 실행합니다.
   <pre><code>ros2 run battery_level_service_server battery_level_service_server </code></pre>
5. bt executor를 실행합니다.
   <pre><code>ros2 run task_manager bt_executor </code></pre>

## How to test
1. bt executor를 실행하여 console에 표시되는 xml파일중 원하는 파일의 번호를 입력하여 실행합니다.
2. battery level은 ros command line tool을 이용하여 변경합니다.
   <pre><code>ros2 topic pub /battery_data std_msgs/msg/UInt32 data:\ test battery level\</code></pre>
   ex : topic pub /battery_data std_msgs/msg/UInt32 data:\ 1\
3. check battery 노드의 battery threshold는 해당 xml에서 변경할수 있습니다.

## Groot 연동하기

### Groot 설치
1. BehaviorTree.CPP의 Dependencies를 설치합니다.
   <pre><code>sudo apt install libzmq3-dev libboost-dev</code></pre>
   
2. BehaviorTree.CPP를 빌드합니다.
   <pre><code>git clone https://github.com/BehaviorTree/BehaviorTree.CPP.git
   cd BehaviorTree.CPP
   mkdir build; cd build
   cmake 
   make
   sudo make install</code></pre>
   
3. Groot의 Dependencies를 설치합니다.
   <pre><code>sudo apt install qtbase5-dev libqt5svg5-dev libzmq3-dev libdw-dev</code></pre>
   
4. Groot를 빌드합니다.
   <pre><code>git clone https://github.com/BehaviorTree/Groot.git
   cd Groot
   git submodule update --init --recursive
   mkdir build; cd build
   cmake ..
   make</code></pre>

### Groot 실행 및 BT 모니터링
1. Groot를 실행합니다.
   <pre><code>cd Groot/build
   ./Groot</code></pre>
2. 실행된 Groot에서 Monitor를 선택합니다.
3. Publisher Port(9999)와 Server Port(9998)을 설정합니다.
4. 왼쪽 탭에 있는 Connect버튼을 눌러서 연결합니다.<br/>
   ***(해당 예제에서는 task_executor를 실행하고 xml을 선택한 이후에 연동이 가능합니다.)***


