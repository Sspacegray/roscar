#include <iostream>
#include <pthread.h>
#include <cstring> // for memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <cstdint>
#include <ros/ros.h>
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h" //注意: 调用 transform 必须包含该头文件
#include "RobotCar/carinfo.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int8MultiArray.h"
#include <tf/tf.h>
#include <jsoncpp/json/json.h>
#include <thread>

typedef enum 
{
    STEP_BEGIN = 0,
    SETP_1,
    SETP_2,
    STEP_OVER,
}RUN_STEP;

// typedef struct HandRun_
// {
//     RUN_STEP  HandRunStep;
//     uint8_t   HandRunStatus;
//     uint8_t   HandRunStatus;
// }

RUN_STEP StepHandRunNum_1 = STEP_BEGIN;
RUN_STEP StepHandRunNum_2 = STEP_BEGIN;

#define HandRunNum_1   1   //
#define HandRunNum_2   2   //

int PutPoint_handflag = 0;

//机械臂
#define SERVER_IP "192.168.1.18" // 服务器IP地址
#define SERVER_PORT 8080       // 服务器端口号

//相机工控机
#define QT_SERVER_IP "192.168.1.10" // 服务器IP地址
#define QT_SERVER_PORT 9090       // 服务器端口号


int PIDnum;


int hand2pid;
int sock;
struct sockaddr_in server_addr;
char server_reply[1024];


int serverSocket;
int clientSocket;




static uint8_t s_buffer[250];

// 0:runing   1 : stop
uint32_t handrunflag =1;
uint32_t downuprunflag =1;
uint32_t qtfinishflag =1;

uint32_t limit_down;
uint32_t limit_up;
int8_t   down_up =0;

 

void qtpaizhao()
{
    // 发送响应
    const char* response = "begin_phoho";
    write(clientSocket, response, strlen(response));
    qtfinishflag = 0;
    
}

void handsetpose(int x,int y,int z,int A,int B,int C)
{
    std::string pose_X,pose_Y,pose_Z,pose_A,pose_B,pose_C,pose_D;
    pose_X = std::to_string(x);
    pose_Y = std::to_string(y);
    pose_Z = std::to_string(z);
    pose_A = std::to_string(A);
    pose_B = std::to_string(B);
    pose_C = std::to_string(C);
    // pose_D = std::to_string(D);

    std::ostringstream cmd_str;
    cmd_str << "{\"command\":\"movej\","
            << "\"joint\":[" << pose_X << ',' << pose_Y << ',' << pose_Z << ',' << pose_A << ',' << pose_B << ',' << pose_C <<  "],"
            << "\"v\":30,"
            << "\"r\":0," 
            << "\"trajectory_connect\":0}";
   std::string cmd =  cmd_str.str() ;


    memset(s_buffer,0,sizeof(s_buffer));
    memcpy(s_buffer,cmd.c_str(),cmd.length());
    // std::cout << cmd.c_str()<<std::endl;
 

    // for(int i=0;i< cmd.length();i++)
    // {
    //     //16进制的方式打印到屏幕
    //     std::cout << std::hex << (s_buffer[i] & 0xff) << " ";
    // }
    // std::cout << std::endl;
    send(sock, s_buffer,cmd.length(),0);
    handrunflag = 0;

}

// 线程要执行的函数
void* threadFunction_2(void* arg) 
{

    while (1)
    {
        ros::param::get("PIDnum",PIDnum);  
        if(PIDnum ==HandRunNum_1)       //到达位置1
        {

            // std::cout << PIDnum  << "   "  << GetPointStep<<   std::endl;
            if(StepHandRunNum_1 == STEP_BEGIN)   
            {
                handsetpose(90000,70000,30000,0,55009,0);;//伸机械臂
                StepHandRunNum_1 =SETP_1;
            }
            else if( StepHandRunNum_1 ==SETP_1)
            {
                if(handrunflag)
                {
                    StepHandRunNum_1 =STEP_OVER;
                    ros::param::set("Enable",true); 
                    
                }
            }

        }
        else  if(PIDnum ==HandRunNum_2) 
        {

            if(StepHandRunNum_2 == STEP_BEGIN)   
            {
                handsetpose(0,-26000,120000,0,85000,0);
                StepHandRunNum_2 =SETP_1;
            }
            else if( StepHandRunNum_2 ==SETP_1)
            {
                if(handrunflag)
                {
                    StepHandRunNum_2 =STEP_OVER;
                    ros::param::set("Enable",true); 
                }
            }
        }
        else if(PIDnum !=HandRunNum_1)
            StepHandRunNum_1 = STEP_BEGIN;
        else if(PIDnum !=HandRunNum_2)
            StepHandRunNum_2 = STEP_BEGIN;

        sleep(1);
    }
}



// 接收机械臂反馈
void* threadFunction(void* arg) 
{
    uint32_t lenth;
    u_int8_t recbuf[500];
    while (1)
    {
        memset(recbuf,0,sizeof(recbuf));
        lenth =recv(sock, recbuf, 500, 0);
        std::cout << "lenth = " << lenth<< std::endl;

        for(int i=89; i<93; i++)
        {
            //16进制的方式打印到屏幕
            std::cout << std::hex << (recbuf[i] & 0xff) << " ";
        }
        std::cout << std::endl;
        if(lenth == 62)
        {
          
            if (recbuf[55] == 0x74  && recbuf[56] == 0x72 && recbuf[57] == 0x75 && recbuf[58] == 0x65 ) 
            {
                handrunflag =1;
                std::cout << "handrunflag = " <<handrunflag << std::endl;
            }
            else 
            {
                handrunflag =0;
                std::cout << "handrunflag = " <<handrunflag << std::endl;
            }
        }
    }
}
void messageCallback(const RobotCar::carinfo& car) 
{
    limit_down = car.limit_down;
    limit_up = car.limit_up;
    // std::cout << "limit_down = "<< limit_down << "  limit_up = " << limit_up << std::endl;
}

void PIDdataCallback(const std_msgs::Int8& PIDdata) 
{
    // PIDnum = PIDdata.data;
    // if(PIDnum == PutPointNum)
    // {
    //     GetPointStep = STEP_BEGIN;
    //     hand2pid =0;
    // }
    // else if(PIDnum ==HandRunNum_1)
    // {
    //     PutPointStep = STEP_BEGIN;
    //     hand2pid =0;
    // }

    //测试
    // else if(PIDnum ==3)
    //    handsetpose(0,-26000,120000,0,85000,0);
    // else if(PIDnum ==4)
    //    handsetpose(90000,70000,30000,0,55009,0);
    // else if(PIDnum ==5)
    //   {
    //     // 发送响应
    //     const char* response = "begin_phoho";
    //     write(clientSocket, response, strlen(response));
    //   }

    // std::cout << "PIDnum =: " << PIDnum << std::endl;
}


//接收相机反馈
void* threadFunction_3(void* arg) 
{
    const int bufferSize = 1024;
    char buffer[bufferSize];
    while(1)
    {

        if(clientSocket > 0)
        { 
             std::cerr << "线程3接受" << std::endl;

            // 接收数据
            ssize_t bytesRead = read(clientSocket, buffer, bufferSize - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // 确保字符串以null结尾
                std::cout << "Received: " << buffer << std::endl;
                // for(int i=0; i<bytesRead; i++)
                // {
                //     //16进制的方式打印到屏幕
                //     std::cout << std::hex << (buffer[i] & 0xff) << " ";
                // }
                // std::cout << std::endl;
                if(buffer[0] == 0x66 && buffer[1] == 0x69 && buffer[2] == 0x6e && buffer[3] == 0x69 )
                {
                    qtfinishflag = 1;
                    std::cout << "qtfinishflag : " << qtfinishflag << std::endl;
                }
                   
            }
            else if(bytesRead ==0)
            {
                std::cout << "Client disconnected" << std::endl;
                clientSocket = -1;
            }
        }

    }
}
// 线程要执行的函数
void* threadFunction_4(void* arg) 
{
    while(1)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Accept failed" << std::endl;
            return 0;
        }
        // 获取客户端的IP地址和端口号
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        // 输出客户端连接成功的提示
        {
            std::cout << "Client connected: IP=" << clientIP << ", Port=" << clientPort << std::endl;
        }

    }
}


int main(int argc, char **argv) {

    pthread_t threadID,threadID_2,threadID_3,threadID_4; // 线程ID
    std::cout << "test_1" << std::endl;
    ros::init(argc, argv, "hand_get");
    ros::NodeHandle n;
    ros::Rate r(10);
 
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);

    ros::Publisher handpub = n.advertise<std_msgs::Int8>("Hand_FLAG",1000);
    ros::Publisher captpub = n.advertise<std_msgs::Int8>("capture_cmd",1000);

    ros::Subscriber sub    = n.subscribe("CarInfo", 1, messageCallback); 
    ros::Subscriber PIDsub = n.subscribe("PID_DATA", 1, PIDdataCallback); 

   /*********************创建与相机通讯服务器************************/
    //
    // serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // if (serverSocket == -1) {
    //     std::cerr << "Could not create socket" << std::endl;
    //     return 1;
    // }
    // struct sockaddr_in serverAddr;
    // serverAddr.sin_family = AF_INET;
    // serverAddr.sin_port = htons(QT_SERVER_PORT);
    // serverAddr.sin_addr.s_addr = inet_addr(QT_SERVER_IP);
    // // 绑定地址到socket
    // if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    //     std::cerr << "Bind failed" << std::endl;
    //     return 1;
    // }
    // // 监听连接
    // listen(serverSocket, 10);

    // std::cout << "Server is running on port 8080" << std::endl;
    /***************************************************************/

   /*********************连接机械臂服务器************************/
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        std::cerr << "Could not create socket" << std::endl;
        return 1;
    }

    // 初始化服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/Address not supported" << std::endl;
        return 1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        std::cerr << "Connect failed" << std::endl;
        return 1;
    }
    /***********************************************************/

    std::cout << "test_2" << std::endl;

    // 创建线程
    int result = pthread_create(&threadID, nullptr, threadFunction, NULL);
    if (result != 0) 
    {
        std::cerr << "线程1创建失败" << std::endl;
        return 1;
    }

        // 创建线程
    int result2 = pthread_create(&threadID_2, nullptr, threadFunction_2, NULL);
    if (result2 != 0) 
    {
        std::cerr << "线程2创建失败" << std::endl;
        return 1;
    }

          // 创建线程
    int result3 = pthread_create(&threadID_3, nullptr, threadFunction_3, NULL);
    if (result3 != 0) 
    {
        std::cerr << "线程3创建失败" << std::endl;
        return 1;
    }
    std::cout << "test_3" << std::endl;
    // // 创建线程
    // int result4 = pthread_create(&threadID_4, nullptr, threadFunction_4, NULL);
    // if (result4 != 0) 
    // {
    //     std::cerr << "线程4创建失败" << std::endl;
    //     return 1;
    // }
    
    
    ros::Rate rate(1);
    while (ros::ok())
    {
        std::cout << "test_4" << std::endl;
        ros::spinOnce();   
        rate.sleep();   
    }
    // 关闭socket
    close(sock);

    // 等待线程结束
    pthread_join(threadID, nullptr);
    return 0;
}


