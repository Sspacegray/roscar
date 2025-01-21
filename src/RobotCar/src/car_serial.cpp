#include<ros/ros.h>
#include<ros/package.h>
#include<geometry_msgs/Twist.h>
#include<iostream>
#include<serial/serial.h>
#include<sstream>
#include<fstream>
#include<stdio.h>
#include<string.h>
#include "RobotCar/carinfo.h"
#include <arpa/inet.h>
#include "std_msgs/Int8.h"



using namespace std;

uint8_t CalcChecksum(uint8_t *pBuf, uint32_t nLength)
{
    uint8_t checksum = 0;
    for(uint8_t i = 0;i < (nLength-1);i++)
    {
        checksum ^= pBuf[i];
    }

    return checksum;
}




#pragma pack (1)
typedef struct _REPORT_DATA_  
{
	unsigned char Head_1;    
    unsigned char Head_2; 
    unsigned char cmd_1; 
    unsigned char cmd_2;         
	int Speed_X;	
    int Speed_Y;				
	int Speed_Z;	

	int16_t acce_x;
	int16_t acce_y;
	int16_t acce_z;
	
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;			
		
    float power;		
	unsigned char  Sum; 				
}REPORT_DATA;


typedef struct _CMD_DATA_  
{
	unsigned char Head_1;    
    unsigned char Head_2; 
    unsigned char cmd_1; 
    unsigned char cmd_2;
    unsigned char mode;
	unsigned int  Speed_X;		
    unsigned int  Speed_Y;	
	unsigned int  Speed_Z;			
	unsigned char  Sum; 				
}CMD_DATA;

#pragma pack ()


//创建一个serial类
serial::Serial sp;

typedef union{   //定义一个共用体，用于double数据与16进制的转换
unsigned char cvalue[4];
float fvalue;
}float_union;

static uint8_t s_buffer[sizeof(CMD_DATA)];
static uint8_t r_buffer[36];
int16_t capturecmd = 0;

void callback(const geometry_msgs::Twist::ConstPtr& cmd_vel)
{
    CMD_DATA *cmd_data = new CMD_DATA;

 	float_union linear_x ,angular_z;
	
	memset(s_buffer,0,sizeof(s_buffer));
    
	linear_x.fvalue = cmd_vel->linear.x;
	angular_z.fvalue = cmd_vel->angular.z;

    cmd_data->Head_1 = 0xA0;
    cmd_data->Head_2 = 0x0A;
    cmd_data->cmd_1  = 0xAA;
    cmd_data->cmd_2  = 0x20;
    cmd_data->mode   = 0x00;
    // ROS_INFO(" push_speed= %x ",cmd_data->push_speed );
    // cmd_data->push_speed = htons(cmd_data->push_speed);

    cmd_data->Speed_X = (int)(cmd_vel->linear.x*1000);
    // cmd_data->Speed_X = htonl(cmd_data->Speed_X);
    cmd_data->Speed_Y = 0x00;
    cmd_data->Speed_Z = (int)(cmd_vel->angular.z*1000);
    // cmd_data->Speed_Z = htonl(cmd_data->Speed_Z);
    // ROS_INFO(" x= %d   z= %d" ,cmd_data->Speed_X,cmd_data->Speed_Z);
    cmd_data->Sum = CalcChecksum((uint8_t*)cmd_data,sizeof(CMD_DATA));
    //cmd_data->Sum = 0xAA;
    // ROS_INFO(" x= %x   x= %d  ",cmd_data->Sum ,sizeof(CMD_DATA));
    memcpy(s_buffer,cmd_data,sizeof(CMD_DATA));
	sp.write(s_buffer,sizeof(CMD_DATA));
    delete cmd_data;

}
void captCallback(const std_msgs::Int8& cmddata) 
{

    capturecmd  = cmddata.data;
    // ROS_INFO(" capturecmd= %x " ,capturecmd);

}

int main(int argc, char** argv)
{
    REPORT_DATA *report_data = new REPORT_DATA();

	ros::init(argc, argv, "cmd_vel_listener");
	ros::NodeHandle n;

  //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/STM32");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(to);
	ROS_INFO("test_1");
    try
    {
        //打开串口
        sp.open();
		ROS_INFO("sp.open");
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }



	
	ros::Publisher pub  = n.advertise<RobotCar::carinfo>("CarInfo",10);
	ros::Subscriber sub = n.subscribe("/cmd_vel", 1000, callback);
    ros::Subscriber captsub = n.subscribe("capture_cmd", 1000, captCallback); 
	ros::Rate loop_rate(10);

	while(n.ok())
	{ 
       //获取缓冲区内的字节数
        size_t n = sp.available();
        if(n!=0)
        {
            uint8_t buffer[1024];
            //读出数据
            n = sp.read(buffer, n);
            
            // for(int i=0; i<n; i++)
            // {
            //     //16进制的方式打印到屏幕
            //     std::cout << std::hex << (buffer[i] & 0xff) << " ";
            // }
            // std::cout << std::endl;
			if(buffer[0] == 0xa0 && buffer[1] == 0x0a && buffer[2] == 0x55 && buffer[3] == 0x99 )
			{
				memcpy(report_data,buffer,sizeof(REPORT_DATA));
                // report_data->Speed_X = htonl(report_data->Speed_X );
                // report_data->Speed_Z = htonl(report_data->Speed_Z );

                // report_data->power = htonl(report_data->power );
				//  ROS_INFO("%d  %f",report_data->Speed_X ,report_data->power );
				//ROS_INFO("%d  %d  %d",report_data->Angel_Acc_X, report_data->Angel_Acc_Y, report_data->Angel_Acc_Z);
			}		
			RobotCar::carinfo car;
			car.speed_x = report_data->Speed_X;
			car.speed_z = report_data->Speed_Z;
            car.power = report_data->power;

			pub.publish(car);         //发布消息
        }
        ros::spinOnce();
		loop_rate.sleep();	
	}
	sp.close();
   // delete report_data;
	return 1;
}