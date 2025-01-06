#include <ros/ros.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <geometry_msgs/Pose.h>
#include <shape_msgs/SolidPrimitive.h>
#include <moveit_msgs/CollisionObject.h>
#include <octomap/octomap.h>
#include <octomap_msgs/Octomap.h>
#include <octomap_msgs/conversions.h>

class OctomapToMoveit {
public:
    OctomapToMoveit(ros::NodeHandle& nh) : planning_scene_interface_() {
        // 订阅Octomap消息
        octomap_sub_ = nh.subscribe("/octomap_binary", 1, &OctomapToMoveit::octomapCallback, this);
    }

    void octomapCallback(const octomap_msgs::OctomapConstPtr& octomap_msg) {
        // 从ROS消息转换Octomap对象
        octomap::AbstractOcTree* tree = octomap_msgs::binaryMsgToMap(*octomap_msg);
        octomap::OcTree* octree = dynamic_cast<octomap::OcTree*>(tree);

        if (!octree) {
            ROS_ERROR("Failed to cast Octree from received Octomap message.");
            return;
        }

        // 处理Octomap中的所有体素
        std::vector<moveit_msgs::CollisionObject> collision_objects;
        int collision_object_count = 0;  // 计数器，统计添加的碰撞物体数量

        for (octomap::OcTree::leaf_iterator it = octree->begin_leafs(), end = octree->end_leafs(); it != end; ++it) {
            if (octree->isNodeOccupied(*it)) {
                moveit_msgs::CollisionObject collision_object;
                collision_object.header.frame_id = "plyy";

                // 获取OcTreeKey的坐标并生成唯一的ID
                octomap::OcTreeKey key = it.getKey();
                std::string key_str = std::to_string(key[0]) + "_" + std::to_string(key[1]) + "_" + std::to_string(key[2]);
                collision_object.id = "octomap_obstacle_" + key_str;  // 通过坐标生成ID

                // 创建一个立方体表示体素
                shape_msgs::SolidPrimitive primitive;
                primitive.type = shape_msgs::SolidPrimitive::BOX;
                primitive.dimensions.resize(3);
                primitive.dimensions[shape_msgs::SolidPrimitive::BOX_X] = octree->getResolution();  // 设置体素的分辨率为立方体的边长
                primitive.dimensions[shape_msgs::SolidPrimitive::BOX_Y] = octree->getResolution();
                primitive.dimensions[shape_msgs::SolidPrimitive::BOX_Z] = octree->getResolution();

                // 设置立方体的位置
                geometry_msgs::Pose pose;
                pose.position.x = it.getX();
                pose.position.y = it.getY();
                pose.position.z = it.getZ();
                pose.orientation.w = 1.0;  // 默认四元数表示无旋转

                // 将物体添加到碰撞场景中
                collision_object.primitives.push_back(primitive);
                collision_object.primitive_poses.push_back(pose);
                collision_object.operation = moveit_msgs::CollisionObject::ADD;
                
                collision_objects.push_back(collision_object);
                collision_object_count++;  // 每添加一个物体，计数器加1
            }
        }

        // 将障碍物添加到MoveIt的规划场景中
        planning_scene_interface_.applyCollisionObjects(collision_objects);

        // 输出添加的碰撞物体数量
        ROS_INFO("Added %d collision objects to the planning scene.", collision_object_count);
    }

private:
    ros::Subscriber octomap_sub_;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;  // 注意使用正确的命名空间
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "octomap_to_moveit");
    ros::NodeHandle nh;

    // 创建OctomapToMoveit对象
    OctomapToMoveit octomap_to_moveit(nh);

    // ROS循环，处理消息
    ros::spin();

    return 0;
}

