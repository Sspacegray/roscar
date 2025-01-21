#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <tf/tf.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h" //注意: 调用 transform 必须包含该头文件
#include <istream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "std_msgs/Int8.h" 
#include "std_msgs/Int8MultiArray.h"
using namespace std;

#define pi 3.1415926

#define PoseRange_XY  0.08
#define AngleRange_Z  0.05


#define GetPointNum   1   //传送带位置
#define PutPointNum   2   //盆栽区域

#define CycleRun      0    

enum RUNSTATE
{
    STOP = 0,
    RUNING,
    FINISH,
};


typedef struct PATH_INFO
{
    u_int8_t IsNeedEnable;
    int      enable;     
    u_int8_t ID_num;
    float    pose_x;
    float    pose_y;
    u_int8_t enable_z;   
    float    pose_z;     //  ±180  = ±pi
    u_int8_t runstate;   //  0:stop  1: running   2 run finish
}path_info;
path_info  pathline[10];

int PIDnum = 0;


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

void handCallback(const std_msgs::Int8MultiArray& cmddata) 
{
    int  num =cmddata.data[0];
    pathline[num].enable = cmddata.data[1];
}



int main(int argc, char **argv)
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "path_pid");
    ros::NodeHandle n;
    ros::Rate r(10);

    
    ros::Publisher pid_pub = n.advertise<std_msgs::Int8>("PID_DATA",1000);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    ros::Publisher path_pub = n.advertise<nav_msgs::Path>("/path_pub",1000);

    ros::Subscriber handsub = n.subscribe("Hand_FLAG", 1000, handCallback); 
    
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);
    nav_msgs::Path nav_path;
    nav_path.header.frame_id = "/map";
    nav_path.header.stamp = ros::Time::now();
    geometry_msgs::PoseStamped path_pose;

    // 读取CSV文件
    std::ifstream csv_data("/home/hy/Robot_ws/move_line.csv", std::ios::in);
    if (!csv_data)
    {
        std::cout << "open .csv failed" << std::endl;
        ROS_ERROR(" .csv  doesn't exisit ");
        std::exit(1);
    }
    geometry_msgs::Quaternion quat;
    std::string line;
    int line_num = 0;
    uint8_t num = 0;
    std::vector<std::string> strbuf;
    geometry_msgs::TransformStamped transformStamped;
    double roll, pitch, yaw; // 定义存储roll,pitch and yaw的容器
    float dif_x, dif_y;
    std_msgs::Int8 msg_num;
    while (ros::ok())
    {
       
        while (std::getline(csv_data, line))
        {
            std::cout << line << std::endl;
            path_pose.header.frame_id = "map";
            path_pose.header.stamp = ros::Time::now();
            path_pose.header.seq = 0;
           
            strbuf = split(line, ",");
            path_pose.pose.position.x = atof(strbuf[0].c_str());
            path_pose.pose.position.y = atof(strbuf[1].c_str());
            path_pose.pose.position.z = 0;

            nav_path.poses.push_back(path_pose);

            pathline[line_num].pose_x         = atof(strbuf[0].c_str());
            pathline[line_num].pose_y         = atof(strbuf[1].c_str());
            pathline[line_num].pose_z         = atof(strbuf[2].c_str());
            pathline[line_num].IsNeedEnable   = atoi(strbuf[3].c_str());
            pathline[line_num].enable_z       = atoi(strbuf[4].c_str());
            pathline[line_num].ID_num = line_num;

            line_num++;
        }

        path_pub.publish(nav_path);//发布路径--rviz端获取
        try
        {
            transformStamped = tfBuffer.lookupTransform("map", "base_footprint", ros::Time(0),ros::Duration(5.0));
            tf::Quaternion quat;
            tf::quaternionMsgToTF(transformStamped.transform.rotation, quat);
            tf::Matrix3x3(quat).getRPY(roll, pitch, yaw); // 进行转换
            // cout<<"current rpy:"<<roll<<", "<<pitch<<", "<<yaw<<endl;
        }
        catch (tf2::TransformException &ex)
        {
            ROS_WARN("%s", ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }

        dif_x = pathline[num].pose_x - transformStamped.transform.translation.x;
        dif_y = pathline[num].pose_y - transformStamped.transform.translation.y;
        ROS_INFO("num: %d  enable_z: %d  runstate: %d ",num,pathline[num].enable_z,pathline[num].runstate);

        geometry_msgs::Twist twist;

        //*********************************************机器人整体控制流程***********************************************//
        if (fabs(dif_x) <= PoseRange_XY && fabs(dif_y) <= PoseRange_XY)//判断目标点坐标距离：到达目标点
        {
            if(pathline[num].enable_z)//判断csv文件中“角度判定”是否赋值
            {
                if(yaw >= pathline[num].pose_z - AngleRange_Z  &&  yaw <= pathline[num].pose_z + AngleRange_Z )//目标角度比较
                {
                    twist.angular.z =0.0;
                    pathline[num].runstate = FINISH;
                }
                else     
                {
                    twist.angular.z = pathline[num].pose_z - yaw;//偏航角纠正
                    pathline[num].runstate = RUNING;
                }
            }
            else//未赋值，原地停车等待指令
            {
                twist.linear.x = 0.0;
                twist.angular.z = 0.0;
                pathline[num].runstate = FINISH;
            }

            if( pathline[num].runstate == FINISH)//到达目标点
            {

                // msg_num.data = num;
                
                if(pathline[num].IsNeedEnable)//判断csv文件中“机器人动作标志位”是否赋值
                {
                    ros::param::get("Enable",pathline[num].enable);  //获取参数服务器中“enable”的值
                    if(pathline[num].enable)//值为1
                    {
                    
                        pathline[num].enable = false ;//值复位
                        ros::param::set("Enable",pathline[num].enable); 
                        num++; 
                    }
                    
                }
                else 
                {
                    num++;//num++,执行下一行目标点
                }
                
            }
            if(CycleRun)
            {
                if (num >= line_num)
                    num = 0;           
            }
            else 
                if (num >= line_num)
                    num = line_num-1; 

            ROS_INFO("num: %d ",num);
            pid_pub.publish(msg_num);
            ros::param::set("PIDnum",num);  //发布当前点位信息，机械臂获取
        }
        else//目标点距离判断：未到达目标点
        {
            double temp = sqrt(pow(dif_x, 2) + pow(dif_y, 2));
            twist.linear.x = 0.5 * sqrt(pow(dif_x, 2) + pow(dif_y, 2));//获取速度

            twist.angular.z = atan2(dif_y, dif_x) - yaw;//获取角度
        }

        if (fabs(twist.angular.z) > pi)//角度限定
        {
            if (twist.angular.z > 0)
                twist.angular.z = twist.angular.z - 2 * pi;
            else if (twist.angular.z < 0)
                twist.angular.z = -twist.angular.z + 2 * pi;
        }

        if (fabs(twist.angular.z) > 0.1)
                twist.linear.x = 0;

        if (twist.linear.x > 0.2)
            twist.linear.x = 0.2;

        if (twist.angular.z > 0.3)
            twist.angular.z = 0.3;
        else if (twist.angular.z < -0.3)
            twist.angular.z = -0.3;
     
        ROS_INFO("twist: %f %f ",twist.linear.x,twist.angular.z);
        pub.publish(twist);//发布速度消息

        // ROS_INFO("test_1");
        ros::Duration(1).sleep();
        ros::spinOnce();
        r.sleep();
    }

    return 0;
}
