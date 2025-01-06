#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/point_cloud.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ply_loader_node");
    ros::NodeHandle nh;

    // 创建一个点云发布者
    ros::Publisher cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("ply_point_cloud", 1);

    // 读取PLY文件
    std::string file_path = "/home/moran/catkin_ws/src/frcobot_ros/frcobot_hw/shelf.ply"; 
    pcl::PointCloud<pcl::PointXYZRGB> cloud;

    if (pcl::io::loadPLYFile(file_path, cloud) == -1)
    {
        PCL_ERROR("Couldn't read the PLY file\n");
        return -1;
    }

    ROS_INFO("Loaded PLY file with %zu points", cloud.size());

    // 将PCL点云转换为ROS消息
    sensor_msgs::PointCloud2 cloud_msg;
    pcl::toROSMsg(cloud, cloud_msg);
    cloud_msg.header.frame_id = "base_link";  // 可以根据需要设置坐标系

    // 发布点云数据
    ros::Rate loop_rate(2);  // 2Hz发布频率
    while (ros::ok())
    {
        cloud_msg.header.stamp = ros::Time::now();  // 更新时间戳
        ROS_INFO("Loaded PLY file with %zu points", cloud.size());
        cloud_pub.publish(cloud_msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

