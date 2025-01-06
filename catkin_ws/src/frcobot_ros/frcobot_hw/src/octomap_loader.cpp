#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <octomap/octomap.h>
#include <octomap_msgs/Octomap.h>
#include <octomap_msgs/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <pcl/filters/voxel_grid.h>

class OctomapLoader {
public:
    OctomapLoader(ros::NodeHandle& nh) {
        // 发布Octomap消息的发布者
        octomap_pub_ = nh.advertise<octomap_msgs::Octomap>("octomap_binary", 1, true);
        pointcloud_sub_ = nh.subscribe("/ply_point_cloud", 1, &OctomapLoader::pointCloudCallback, this);
        
        // 创建Octomap对象，分辨率设置 
        tree_ = std::make_shared<octomap::OcTree>(0.05);
    }

    void pointCloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud_msg) {
        // 将PointCloud2消息转换为PCL的点云数据
        pcl::PointCloud<pcl::PointXYZRGB> pcl_cloud;
        pcl::fromROSMsg(*cloud_msg, pcl_cloud);

        // 将点云数据添加到Octomap
        for (const auto& point : pcl_cloud.points) {
            // 将每个点添加到Octomap，设为占据状态（true）
            tree_->updateNode(octomap::point3d(point.x, point.y, point.z), true);
        }

        // 发布Octomap
        publishOctomap();
    }

    void publishOctomap() {
        // 将Octomap转换为ROS消息
        octomap_msgs::Octomap octomap_msg;
        octomap_msgs::binaryMapToMsg(*tree_, octomap_msg);
        
        // 设置消息的时间戳和坐标系
        octomap_msg.header.stamp = ros::Time::now();
        octomap_msg.header.frame_id = "plyy";  // 可以根据需要设置坐标系

        // 发布Octomap
        octomap_pub_.publish(octomap_msg);
    }

private:
    ros::Publisher octomap_pub_;
    ros::Subscriber pointcloud_sub_;
    std::shared_ptr<octomap::OcTree> tree_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "octomap_loader_node");
    ros::NodeHandle nh;

    // 创建OctomapLoader对象
    OctomapLoader loader(nh);

    // ROS循环，处理消息
    ros::spin();

    return 0;
}
