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
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "std_srvs/Empty.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"

using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// 机器人状态枚举
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

// 决策树节点基类
class DecisionNode {
public:
    virtual bool evaluate(const robot_info& robot) = 0;
    virtual ROBOT_STATE getNextState() = 0;
    virtual ~DecisionNode() {}
};

// 具体决策节点：检查是否在4-10房间区域
class InRoomAreaNode : public DecisionNode {
public:
    bool evaluate(const robot_info& robot) override {
        return robot.last_point_num >= 4 && robot.last_point_num <= 10;
    }
    
    ROBOT_STATE getNextState() override {
        return START;  // 实际状态将由子节点决定
    }
};

// 具体决策节点：检查目标是否在4-10房间区域
class GoalInRoomAreaNode : public DecisionNode {
public:
    bool evaluate(const robot_info& robot) override {
        return robot.goal_point_num >= 4 && robot.goal_point_num <= 10;
    }
    
    ROBOT_STATE getNextState() override {
        return START;  // 实际状态将由子节点决定
    }
};

// 导航决策树
class NavigationDecisionTree {
private:
    MoveBaseClient& client_;
    vector<move_base_msgs::MoveBaseGoal>& goals_;
    
public:
    NavigationDecisionTree(MoveBaseClient& client, vector<move_base_msgs::MoveBaseGoal>& goals)
        : client_(client), goals_(goals) {}
    
    ROBOT_STATE makeDecision(robot_info& robot) {
        // 检查是否需要停止
        if (robot.goal_point_num > 13) {
            ROS_INFO("Invalid room point: %d", robot.goal_point_num);
            robot.robot_start_flag = 0;
            ros::param::set("ROBOT_start", robot.robot_start_flag);
            return STOP;
        }
        
        // 决策树主逻辑
        GoalInRoomAreaNode goalInRoom;
        InRoomAreaNode robotInRoom;
        
        bool isGoalInRoom = goalInRoom.evaluate(robot);
        bool isRobotInRoom = robotInRoom.evaluate(robot);
        
        // 决策逻辑
        if (isGoalInRoom) {
            if (isRobotInRoom) {
                // 都在房间区域内，直接导航
                ROS_INFO("Direct navigation to goal");
                client_.sendGoal(goals_[robot.goal_point_num]);
                return RUNNING;
            } else {
                // 目标在房间内，机器人在外，需要经过A点
                ROS_INFO("Going to A point to enter room area");
                client_.sendGoal(goals_[2]);
                return A_OPEN_DOOR;
            }
        } else {
            if (isRobotInRoom) {
                // 目标在外，机器人在内，需要经过B点
                ROS_INFO("Going to B point to exit room area");
                client_.sendGoal(goals_[3]);
                return B_OPEN_DOOR;
            } else {
                // 都在房间区域外，直接导航
                ROS_INFO("Direct navigation to goal outside room area");
                client_.sendGoal(goals_[robot.goal_point_num]);
                return RUNNING;
            }
        }
    }
    
    ROBOT_STATE handleRunningState() {
        client_.waitForResult();
        if (client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("Goal reached");
            return FINISH;
        } else {
            ROS_INFO("Goal failed");
            return FAILED;
        }
    }
    
    ROBOT_STATE handleDoorOperation(bool isOpeningA) {
        client_.waitForResult();
        if (client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("%s", isOpeningA ? "Opening door at A" : "Opening door at B");
            ros::Duration(10.0).sleep();
            
            // 移动到对面的门点
            client_.sendGoal(goals_[isOpeningA ? 3 : 2]);
            ROS_INFO("Moving to opposite door point to close");
            return CLOSE_DOOR;
        } else {
            ROS_INFO("Door operation failed");
            return FAILED;
        }
    }
    
    ROBOT_STATE handleCloseDoor(robot_info& robot) {
        client_.waitForResult();
        if (client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("Closing door");
            ros::Duration(3.0).sleep();
            client_.sendGoal(goals_[robot.goal_point_num]);
            return RUNNING;
        } else {
            return FAILED;
        }
    }
};

// 机器人控制器类
class RobotController {
private:
    ros::NodeHandle nh_;
    ros::ServiceClient global_localization_client_;
    ros::Publisher initial_pose_pub_;
    tf2_ros::Buffer tfBuffer_;
    tf2_ros::TransformListener tfListener_;
    NavigationDecisionTree* decisionTree_;
    MoveBaseClient* client_;
    vector<move_base_msgs::MoveBaseGoal> goals_;
    
public:
    RobotController() : tfListener_(tfBuffer_) {
        // 初始化导航客户端
        client_ = new MoveBaseClient("move_base", true);
        client_->waitForServer();
        
        // 初始化决策树
        decisionTree_ = new NavigationDecisionTree(*client_, goals_);
        
        // 初始化AMCL服务
        global_localization_client_ = nh_.serviceClient<std_srvs::Empty>("/global_localization");
        initial_pose_pub_ = nh_.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 1);
        
        // 等待tf树建立
        waitForTFTree();
    }
    
    ~RobotController() {
        delete decisionTree_;
        delete client_;
    }
    
    void waitForTFTree() {
        ros::Duration(1.0).sleep();
        try {
            tfBuffer_.lookupTransform("map", "base_link", ros::Time(0), ros::Duration(5.0));
            ROS_INFO("TF tree is ready");
        } catch (tf2::TransformException &ex) {
            ROS_WARN("TF tree not ready yet: %s", ex.what());
        }
    }
    
    ROBOT_STATE updateState(robot_info& robot) {
        switch (robot.robot_state) {
            case START:
                return decisionTree_->makeDecision(robot);
            case RUNNING:
                return decisionTree_->handleRunningState();
            case A_OPEN_DOOR:
                return decisionTree_->handleDoorOperation(true);
            case B_OPEN_DOOR:
                return decisionTree_->handleDoorOperation(false);
            case CLOSE_DOOR:
                return decisionTree_->handleCloseDoor(robot);
            case FINISH:
                handleFinishState(robot);
                return robot.robot_state;
            case WAITTING:
                handleWaitingState(robot);
                return robot.robot_state;
            case BACK_TO_0:
                handleBackToChargingState(robot);
                return robot.robot_state;
            case FAILED:
                handleFailureState(robot);
                return robot.robot_state;
            default:
                return robot.robot_state;
        }
    }
    
    void handleFinishState(robot_info& robot) {
        robot.last_point_num = robot.goal_point_num;
        robot.room_fresh_falg = 0;
        if (robot.goal_point_num == 0) {
            robot.robot_start_flag = 0;
            ros::param::set("ROBOT_start", robot.robot_start_flag);
        } else {
            robot.robot_state = WAITTING;
        }
    }
    
    void handleWaitingState(robot_info& robot) {
        static ros::Time start_wait_time;
        static bool is_waiting = false;
        
        if (!is_waiting) {
            start_wait_time = ros::Time::now();
            is_waiting = true;
        }
        
        if (robot.room_fresh_falg) {
            robot.robot_state = START;
            is_waiting = false;
        } else if ((ros::Time::now() - start_wait_time).toSec() > 120) {
            ROS_INFO("No goal received for 120s, returning to charging point");
            robot.robot_state = BACK_TO_0;
            is_waiting = false;
        }
    }
    
    void handleBackToChargingState(robot_info& robot) {
        ROS_INFO("Returning to charging point");
        robot.goal_point_num = 0;
        ros::Duration(1.0).sleep();
        robot.robot_state = START;
    }
    
    void handleFailureState(robot_info& robot) {
        // 实现失败恢复策略
        ROS_ERROR("Navigation failed, implementing recovery strategy");
        // 可以添加重试逻辑或返回充电点
    }
    
    // ... 其他方法保持不变 ...
};

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "robot_controller");
    
    RobotController controller;
    robot_info robot;
    
    // 初始化机器人状态
    robot.robot_state = STOP;
    robot.robot_start_flag = 0;
    robot.goal_point_num = 0;
    robot.last_point_num = 0;
    
    ros::Rate rate(5);
    
    while(ros::ok()) {
        if(robot.robot_state == STOP && robot.robot_start_flag) {
            robot.robot_state = START;
        } else if(robot.robot_state != STOP && robot.robot_start_flag == 0) {
            robot.robot_state = STOP;
        }
        
        robot.robot_state = controller.updateState(robot);
        ros::spinOnce();
        rate.sleep();
    }
    
    return 0;
}
