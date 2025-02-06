#include "ros/ros.h"
#include "RobotCar/ClearDynamicObstacles.h"
#include <costmap_2d/costmap_2d_ros.h>
#include <costmap_2d/costmap_2d.h>
#include <tf2_ros/transform_listener.h>

/**
 * @brief 清除动态障碍物服务节点
 * 提供清除全部动态障碍物或指定ID障碍物的服务
 */
class ClearDynamicObstaclesServer
{
public:
    ClearDynamicObstaclesServer(tf2_ros::Buffer& tf) : 
        costmap_ros_("global_costmap", tf)
    {
        service_ = nh_.advertiseService("clear_dynamic_obstacles", 
            &ClearDynamicObstaclesServer::clearObstaclesCallback, this);
        ROS_INFO("Clear dynamic obstacles service started");
    }

private:
    bool clearObstaclesCallback(RobotCar::ClearDynamicObstacles::Request &req,
                               RobotCar::ClearDynamicObstacles::Response &res)
    {
        try {
            costmap_2d::Costmap2D* costmap = costmap_ros_.getCostmap();
            if (!costmap) {
                res.success = false;
                res.message = "Failed to get costmap pointer";
                ROS_ERROR("%s", res.message.c_str());
                return true;
            }

            if(req.clear_all) {
                ROS_INFO("Clearing all dynamic obstacles");
                costmap->resetDynamicObstacles();
                res.success = true;
                res.message = "Cleared all dynamic obstacles";
            }
            else {
                if(req.obstacle_ids.empty()) {
                    res.success = false;
                    res.message = "No obstacle IDs provided";
                    ROS_WARN("%s", res.message.c_str());
                    return true;
                }

                ROS_INFO("Clearing specific obstacles: %lu ids", req.obstacle_ids.size());
                for(const auto& id : req.obstacle_ids) {
                    if(!costmap->removeDynamicObstacle(id)) {
                        res.success = false;
                        res.message = "Failed to remove obstacle with id: " + id;
                        ROS_ERROR("%s", res.message.c_str());
                        return true;
                    }
                    ROS_DEBUG("Removed obstacle with id: %s", id.c_str());
                }
                res.success = true;
                res.message = "Successfully removed specified obstacles";
            }
        }
        catch(const std::exception& e) {
            res.success = false;
            res.message = "Exception occurred: " + std::string(e.what());
            ROS_ERROR("Exception in clearObstaclesCallback: %s", e.what());
            return true;
        }
        
        ROS_INFO("%s", res.message.c_str());
        return true;
    }

    ros::NodeHandle nh_;
    ros::ServiceServer service_;
    costmap_2d::Costmap2DROS costmap_ros_;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "clear_dynamic_obstacles_server");
    
    try {
        tf2_ros::Buffer tfBuffer;
        tf2_ros::TransformListener tfListener(tfBuffer);
        
        ClearDynamicObstaclesServer server(tfBuffer);
        ROS_INFO("Clear dynamic obstacles server is ready");
        
        ros::spin();
    }
    catch(const std::exception& e) {
        ROS_ERROR("Exception in main: %s", e.what());
        return 1;
    }
    
    return 0;
}
