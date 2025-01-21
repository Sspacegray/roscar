
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"
#include "RobotCar/carinfo.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "RobotCar/web2robot.h"
#include <tf/transform_datatypes.h>

using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

enum ROBOT_STATE
{   
    STOP     = 0,
    START    = 1,
    RUNNING  = 2,
    FINISH   = 3,
    WAITTING = 4,
    A_OPEN_DOOR = 5,
    B_OPEN_DOOR = 6,
    CLOSE_DOOR = 7,
    BACK_TO_0 = 8,
    FAILED    = 9,
};

typedef struct ROBOT_INFO
{
    u_int8_t robot_state;//机器人状态
    u_int8_t room_fresh_falg;
    int robot_start_flag;//机器人开始工作标志位

    int goal_point_num;//机器人目标点
    int last_point_num;//机器人上一个目标点

}robot_info;

robot_info robot;

move_base_msgs::MoveBaseGoal goal[40];//创建goal房间坐标点数组

std::vector<std::string> split(const std::string &str, const std::string &pattern)
{
    char *strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str()); // string转换成C-string
    std::vector<std::string> res;
    char *temp = strtok(strc, pattern.c_str());
    while (temp != NULL)
    {
        res.push_back(std::string(temp));
        temp = strtok(NULL, pattern.c_str());
    }
    delete[] strc;
    return res;
}

bool web2robot_callback(RobotCar::web2robot::Request& req,RobotCar::web2robot::Response& resp)
{   
    robot.robot_start_flag = req.robot_start;    
    if(robot.robot_start_flag == 0)
        ROS_INFO("ROBOT stop");
    else
        ROS_INFO("ROBOT start");
    
    
    if(req.room_point==2||req.room_point==3)
    {
        ROS_ERROR("room_point is out of range!");
        return false;
    }
    else
    {
        robot.goal_point_num = req.room_point;
        robot.room_fresh_falg = 1;//房间刷新
    }
    resp.goal_point = robot.goal_point_num;
    return true;
}

void handleStartState(MoveBaseClient& client)
{
    ROS_INFO("goal point:%d", robot.goal_point_num);
    if(robot.goal_point_num > 13)//目标点超出房间号
    {
        ROS_INFO("DIDN`T find this room point: %d", robot.goal_point_num);
        robot.robot_start_flag = 0;
        ros::param::set("ROBOT_start",robot.robot_start_flag);//机器人返回充电位，停止
    }
    else
    {
        if(robot.goal_point_num <= 10 && robot.goal_point_num >= 4)//目标点在4-10中
        {
            if(robot.last_point_num <= 10 && robot.last_point_num >= 4)//机器人当前位置在4-10中，直接前往目标点
            {
                ROS_INFO("goal has sended");
                client.sendGoal(goal[robot.goal_point_num]);//发送目标值
                robot.robot_state = RUNNING;
            }
            else//机器人当前位置不在4-10中，先去开门A点
            {
                ROS_INFO("go to A to open door"); 
                client.sendGoal(goal[2]);//发送开门点A目标值
                robot.robot_state = A_OPEN_DOOR;
            }
        }
        else//目标点不在4-10中
        {
            if(robot.last_point_num <= 10 && robot.last_point_num >= 4)//机器人当前位置在4-10中，先去B开门
            {
                ROS_INFO("go to B to open door");
                client.sendGoal(goal[3]);//发送开门点B目标值
                robot.robot_state = B_OPEN_DOOR;
            }
            else//机器人当前位置不在4-10中，直接前往目标点
            {
                ROS_INFO("goal has sended");
                client.sendGoal(goal[robot.goal_point_num]);//发送目标值
                robot.robot_state = RUNNING;
            }
        }
    }
}

void handleRunningState(MoveBaseClient& client)
{
    client.waitForResult();
    if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        robot.robot_state = FINISH;
        ROS_INFO("goal reached");
    }
    else
    {
        ROS_INFO("Goal failed");
        robot.robot_state = FAILED;
    }

}

void handle_A_DoorOpenState(MoveBaseClient& client)
{
    client.waitForResult();
    if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        //open_door();//开门
        ROS_INFO("open door");
        ros::Duration(10.0).sleep();//等待10s
        client.sendGoal(goal[3]);//开门后前往B点
        ROS_INFO("go to B to close door");
        robot.robot_state = CLOSE_DOOR;
        robot.last_point_num = 3;//刷新机器人上一次目标点为关门点
    }
    else
    {
        ROS_INFO("Goal failed");
        robot.robot_state = FAILED;
    }
}

void handle_B_OpenDoorState(MoveBaseClient& client)
{
    client.waitForResult();
    if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        //open_door();//开门
        ROS_INFO("open door");
        ros::Duration(10.0).sleep();//等待10s
        client.sendGoal(goal[2]);//开门后前往A点
        ROS_INFO("go to A to close door");
        robot.robot_state = CLOSE_DOOR;
        robot.last_point_num = 2;//刷新机器人上一次目标点为关门点
    }
    else
    {
        ROS_INFO("Goal failed");
        robot.robot_state = FAILED;
    }
}

void handleCloseDoorState(MoveBaseClient& client)
{
    client.waitForResult();
    if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        //close_door();//关门
        ROS_INFO("close door");
        ros::Duration(3.0).sleep();//等待3s
        client.sendGoal(goal[robot.goal_point_num]);//关门后前往目标点
        ROS_INFO("go to goal");
        robot.robot_state = RUNNING;
    }
    else
    {
        ROS_INFO("Goal failed");
        robot.robot_state = FAILED;
    }
}

void handleFinishState()
{
    robot.last_point_num = robot.goal_point_num;//刷新机器人上一次目标点
    robot.room_fresh_falg = 0;//房间刷新
    if(robot.goal_point_num == 0)//目标点是充电位
    {
        robot.robot_start_flag = 0;
        ros::param::set("ROBOT_start",robot.robot_start_flag);//机器人返回充电位，停止
    }
    else
    {
        robot.robot_state = WAITTING;//机器人进入等待状态，等待下一次任务下发
    }
}
bool ismove= false;
ros::Time start_wait_time;
void handleWaitingState()
{
    if(!ismove){
        start_wait_time = ros::Time::now();
        ismove = true;
    }
    if(robot.room_fresh_falg)//目标点更新
    {
        robot.robot_state = START;
        ismove = false;
    }
    else if((ros::Time::now() - start_wait_time).toSec() > 120)//300s内没有下发任务
    {
        ROS_INFO("waitting for 300s,NO GOAL FRESH");
        robot.robot_state = BACK_TO_0; 
        ismove = false;
    }
}

void handleBackToChargingState()
{
    ROS_INFO("back to charge_point");
    robot.goal_point_num = 0;//目标点设为0，返回充电位
    ros::Duration(1.0).sleep();
    robot.robot_state = START;//置为start，机器人开始规划返回路线
}

void handleFailureState()
{
    //尝试让机器人返回充电位
    // robot.goal_point_num = 0;//目标点设为0，返回充电位
    // robot.robot_state = START;//置为start，机器人开始规划返回路线
}

void RobotStateManager(MoveBaseClient& client)
{
     switch (robot.robot_state)
        {
            case START:
                handleStartState(client);
                break;
            case RUNNING:
                handleRunningState(client);
                break;
            case A_OPEN_DOOR:
                handle_A_DoorOpenState(client);
                break;
            case B_OPEN_DOOR:
                handle_B_OpenDoorState(client);
                break;
            case CLOSE_DOOR:
                handleCloseDoorState(client);
                break;
            case FINISH:
                handleFinishState();
                break;
            case WAITTING:
                handleWaitingState();
                break;
            case BACK_TO_0:
                handleBackToChargingState();
                break;
            case STOP:
                break;
            case FAILED:
                handleFailureState();//机器人导航失败，此处应该需要更加完整的错误处理方法，目前只尝试让机器人返回充电位
                break;
            default:
                ROS_ERROR("Unknown state");
                break;
        }
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "robot_controller");
    ros::NodeHandle nh;
    ros::ServiceServer server = nh.advertiseService("WEB2ROBOT_server",web2robot_callback);
    //读取标定的目标点坐标csv文件
    std::ifstream csv_data("/home/hy/Robot_ws/points.csv", std::ios::in);
    if (!csv_data)
    {
        std::cout << "open .csv failed" << std::endl;
        ROS_ERROR(" .csv  doesn't exisit ");
        std::exit(1);
    }
    std::string line;
    std::vector<std::string> strbuf;
    int num = 0;

    while (std::getline(csv_data, line))//读取房间坐标
    {
        std::cout << line << std::endl;
        // double yaw = atof(strbuf[3].c_str());

        strbuf = split(line, ",");
    goal[num].target_pose.header.frame_id = "odom_combined";
    goal[num].target_pose.header.stamp = ros::Time::now();
    goal[num].target_pose.pose.position.x = atof(strbuf[1].c_str());
    goal[num].target_pose.pose.position.y = atof(strbuf[2].c_str());
    goal[num].target_pose.pose.orientation.z = atof(strbuf[3].c_str());
    goal[num].target_pose.pose.orientation.w = atof(strbuf[4].c_str());
    
        num++;
    }
    MoveBaseClient client("move_base",true);//ros初始化

    client.waitForServer();//等待服务器启动
    ROS_INFO("server started");

    robot.robot_state = STOP;
    robot.robot_start_flag  = 0;
    robot.goal_point_num = 0;
    robot.last_point_num = 0;
    ros::Rate rate(5);
    while(ros::ok())
    {
        if(robot.robot_state == STOP)
        {
            if(robot.robot_start_flag)
            robot.robot_state = START;
        }
        else
        {
            if(robot.robot_start_flag == 0)
            robot.robot_state = STOP;
        }

        RobotStateManager(client);
        // ROS_INFO("robot state:%d",robot.robot_state);
        ros::spinOnce();
    }

    return 0;
}
