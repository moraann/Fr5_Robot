#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/point_cloud.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ply_loader_node");
    ros::NodeHandle nh;

    // 创建一个点云发布者
    ros::Publisher cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("ply_point_cloud", 1);

    // 创建tf2广播器
    //tf2_ros::TransformBroadcaster tf_broadcaster;

    // 读取PLY文件
    std::string file_path = "/home/moran/catkin_ws/src/frcobot_ros/frcobot_hw/shelf.ply";  // 替换为你的PLY文件路径
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

    // 设置点云原始坐标系为 "plyy"
    cloud_msg.header.frame_id = "plyy";

    /*
    // 发布从 "plyy" 到 "base_link" 的静态变换
    geometry_msgs::TransformStamped transformStamped;
    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "plyy";  // 源坐标系是 "plyy"
    transformStamped.child_frame_id = "world";  // 目标坐标系是 "world"

    // 设置平移和旋转
    transformStamped.transform.translation.x = 0.15556;  // +left and -right
    transformStamped.transform.translation.y = 0.22223;  // +up and -down
    transformStamped.transform.translation.z = -0.83223; // -forward and +backward

    transformStamped.transform.rotation.x = 0.0;  //0.0;
    transformStamped.transform.rotation.y = 0.0;  //0.0
    transformStamped.transform.rotation.z = 0.0;	//-0.7071
    transformStamped.transform.rotation.w = 1.0;  // 0.7071

    // 发布变换
    tf_broadcaster.sendTransform(transformStamped);
	
    */
    // 发布点云数据
    ros::Rate loop_rate(2);  // 2Hz 发布频率
    while (ros::ok())
    {
        cloud_msg.header.stamp = ros::Time::now();  // 更新时间戳
        cloud_pub.publish(cloud_msg);  // 发布点云
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
