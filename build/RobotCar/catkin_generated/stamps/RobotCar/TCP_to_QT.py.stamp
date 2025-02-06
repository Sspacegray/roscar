#! /usr/bin/env python

"""
    Python 版 HelloWorld

"""
import random
import socket
import sys
import threading
import time
import rospy
import tf2_ros


import rospy
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import OccupancyGrid
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Twist

from RobotCar.msg import carinfo

sys.path.append("/opt/ros/noetic/lib/python3/dist-packages/")
sys.path.append("/home/agrobot/Desktop/Robot_ws/src")

# Global variables
map_width = None
map_height = None
map_data = None
x_value = None
y_value = None
theta_value = None
map_origin_x = None
map_origin_y = None
mypose = None

def cmd_vel_control(command):
    pub_cmd_vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    cmd_vel_msg = Twist()

    if command == 'go_up':
        cmd_vel_msg.angular.z = 0
        cmd_vel_msg.linear.x = 0.2
    elif command == 'go_back':
        cmd_vel_msg.angular.z = 0
        cmd_vel_msg.linear.x = -0.2
    elif command == 'turn_right':
        cmd_vel_msg.linear.x = 0
        cmd_vel_msg.angular.z = -0.2
    elif command == 'turn_left':
        cmd_vel_msg.linear.x = 0
        cmd_vel_msg.angular.z = 0.2
    elif command == 'pause_work' or command == 'stop_robot':
        cmd_vel_msg.angular.z = 0
        cmd_vel_msg.linear.x = 0

    pub_cmd_vel.publish(cmd_vel_msg)



def tcp_command(command):
    if command == 'connect_robot':
        return 'robot_connected'
    elif command == 'stop_robot':
        cmd_vel_control(command)
        time.sleep(1)
        return 'robot_stopped'
    elif command == 'start_work':
        print('******&&&&&&&&^^^^^^^^^^^%%%%%%')
        print('******&&&&&&&&^^^^^^^^^^^%%%%%%')
        print('******&&&&&&&&^^^^^^^^^^^%%%%%%')
        print('******&&&&&&&&^^^^^^^^^^^%%%%%%')
        ros_pose_pub_thread = threading.Thread(target=ros_pose_publisher)
        ros_pose_pub_thread.daemon = True
        ros_pose_pub_thread.start()
        return 'work_starting'
    elif command == 'pause_work':
        cmd_vel_control(command)
        return 'work_paused'
    elif command == 'go_up':
        cmd_vel_control(command)
        return 'going_up'
    elif command == 'go_back':
        cmd_vel_control(command)
        return 'going_back'
    elif command == 'turn_right':                          
        cmd_vel_control(command)
        return 'turning_right'
    elif command == 'turn_left':
        cmd_vel_control(command)
        return 'turning_left'
    elif command == 'rebuild_map':  # map_upload
        if map_data is not None:
            print(map_width * map_height)
            modified_data = [2 if x == -1 else x for x in map_data]
            return f'map,{map_width},{map_height},{"".join(map(str, modified_data))}!'
        else:
            return 'no_map_data'
    elif command == 'world_pose':
        world_pose = 'world_pose' + ',' + str(map_origin_x) + ',' + str(map_origin_y)
        print(f'Response pose: {world_pose}')
        return world_pose
    elif command == 'pose_send':
        pose_inform = 'recv_pose' + ',' + str(x_value) + ',' + str(y_value) + ',' + str(theta_value) + ',' + str(power)
        print(f'Response pose: {pose_inform}')
        return pose_inform
    else:
        split_data = command.split(',') 
        cmd_str = split_data[0]
        if cmd_str == 'move_point':
            pose_x = float(split_data[1])
            pose_y = float(split_data[2])
            pose_z = float(split_data[3])

            turtle_vel_pub = rospy.Publisher('move_base_simple/goal', PoseStamped, queue_size=1)  
            mypose=PoseStamped()
            turtle_vel_pub.publish(mypose) #先发送一个空位置，试探一下，否则第一个包容易丢
            time.sleep(1)     

            mypose=PoseStamped()
            mypose.header.frame_id='map' #设置自己的目标
            mypose.pose.position.x=pose_x
            mypose.pose.position.y=pose_y
            mypose.pose.position.z=0
            mypose.pose.orientation.x=0
            mypose.pose.orientation.y=0
            mypose.pose.orientation.z=0
            mypose.pose.orientation.w=1
            turtle_vel_pub.publish(mypose) #发送自己设置的目标点
            time.sleep(5)


            print(f"cmd_str:  {cmd_str} ")
            print(f"pose_x:  {pose_x} ")
            print(f"pose_y:  {pose_y} ")
            print(f"pose_z:  {pose_z} ")
            print(f'Received: {command}')
            return command
        elif cmd_str == 'move_line':
            fo = open('/home/hy/Robot_ws/move_line.csv', 'w')
            print("文件名为: ", fo.name)
            point_num = int(split_data[1])
            i = 0
            while i < point_num*2:
                line_point_x = split_data[i+2]   
                line_point_y = split_data[i+3]
                point_str = str(line_point_x) + ',' + str(line_point_y)+',\n'#每个点占一行
                fo.write( point_str )
                i+=2

            fo.close()
            return command
        else:
            return 'unknow_command'

def handle_client(conn, addr):
    print(f'Connected by {addr}')
    with conn:
        while True:
            try:
                data = conn.recv(1024)
                if not data:
                    print(f"No data received. Connection with {addr} might be closed.")
                    break
                command = data.decode().strip()
                print(f'Received: {command}')
                response = tcp_command(command)
                print(f'Response Length: {len(response)}')
                conn.sendall(response.encode('utf-8'))
                if command == 'stop_robot':
                    ros_subprocess.close_port()
            except socket.timeout:
                print(f"Connection with {addr} timed out.")
                break
            except socket.error as e:
                print(f'Socket error: {e}')
                break
            except Exception as e:
                print(f'Error: {e}')
                break
    print(f'Connection with {addr} closed')




# def start_tcp_server(host='192.168.85.253', port=9005):
def start_tcp_server(host='192.168.124.41', port=9005):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
        s.bind((host, port))
        s.listen()
        print(f'Server listening on {host}:{port}')

        while True:
            try:
                conn, addr = s.accept()
                conn.settimeout(600)
                handle_client(conn, addr)
            except socket.timeout:
                print("Server socket timed out waiting for a connection.")
            except Exception as e:
                print(f"Error accepting connection: {e}")


def map_callback(data):
    global map_width, map_height, map_data, map_origin_x, map_origin_y
    map_width = data.info.width
    map_height = data.info.height
    map_resolution = data.info.resolution
    map_data = list(data.data)
    map_origin_x = data.info.origin.position.x
    map_origin_y = data.info.origin.position.y
    

def carinfo_callback(data):
    global power
    power = data.power
    # rospy.loginfo("%d  %d  %d ",data.speed_x,data.speed_z,data.power)  



# def pose_callback(msg):
#     global x_value, y_value, theta_value
#     x_value = msg.pose.pose.position.x
#     y_value = msg.pose.pose.position.y
#     x_value = int(x_value)
#     y_value = int(y_value)
#     theta_value = msg.pose.pose.orientation.w

def ros_inform_sub_listener():
    global x_value, y_value, theta_value
    buffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(buffer)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():

        rate.sleep()
        try:
            #def lookup_transform(self, target_frame, source_frame, time, timeout=rospy.Duration(0.0)):
            trans = buffer.lookup_transform("map","base_footprint",rospy.Time(0),rospy.Duration(5.0))
            x_value = trans.transform.translation.x
            y_value = trans.transform.translation.y
            theta_value = trans.transform.rotation.z
            # rospy.loginfo("相对坐标:(%.2f,%.2f,%.2f)", x_value,y_value,theta_value )   
        except Exception as e:
            rospy.logwarn("警告:%s",e)




def main(ros_command_list):
    rospy.init_node("listener_p")

    rospy.Subscriber("/map", OccupancyGrid, map_callback)
    rospy.Subscriber("/CarInfo", carinfo, carinfo_callback)
 
    # source
    # ros_subprocess.start_ros_node(ros_command_list['source_command'])
    # TCP_thread
    tcp_server_thread = threading.Thread(target=start_tcp_server)
    tcp_server_thread.daemon = True
    tcp_server_thread.start()

    # ros_master_start
    # ros_subprocess.start_ros_node(ros_command_list['master_command'])
    # rospy.init_node("argi_robot_master", anonymous=True)
    # time.sleep(8)

    # ros_pose_sub_thread
    ros_pose_sub_thread = threading.Thread(target=ros_inform_sub_listener)
    ros_pose_sub_thread.daemon = True
    ros_pose_sub_thread.start()


    # keep_alive
    tcp_server_thread.join()
    ros_pose_sub_thread.join() 
    rospy.spin()
   





if __name__ == "__main__":

    # # ROS_commands
    ros_commands = {
        # 'master_command': '/opt/ros/noetic/bin/roslaunch turn_on_agri_robot navigation.launch navigation:=true',
        #  'master_command': '/opt/ros/noetic/bin/roslaunch RobotCar RobotcCar.launch',
        # 'source_command': 'source ./devel/setup.bash',
        # 'amcl_node': '/opt/ros/noetic/bin/roslaunch turn_on_agri_robot amcl.launch',
    }
    main(ros_commands)


