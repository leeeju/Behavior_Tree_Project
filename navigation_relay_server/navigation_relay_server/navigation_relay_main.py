import time
from navigation_relay_server.navigation_relay_class import NavigationRelayServer


def main():
    node_instance = NavigationRelayServer()
    node_instance.start()
    print("NavigationRelayServer run!")
    while True:
        node_instance.ros_spin_once()
        time.sleep(0.125)


if __name__ == '__main__':
    main()
