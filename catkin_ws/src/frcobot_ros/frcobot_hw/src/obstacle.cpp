#include <ros/ros.h>
#include <moveit_commander/moveit_commander.h>
#include <moveit_commander/planning_scene_interface.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <shape_msgs/Mesh.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
  // 初始化ROS节点
  ros::init(argc, argv, "add_mesh_obstacle_node");
  ros::NodeHandle nh;

  // 初始化 MoveIt Commander
  moveit_commander::init(argc, argv);

  // 创建规划场景接口
  moveit_commander::PlanningSceneInterface planning_scene_interface;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  // tf2 广播器
  tf2_ros::TransformBroadcaster tf_broadcaster;

  // 文件路径
  std::string mesh_file = "/home/moran/catkin_ws/src/frcobot_ros/frcobot_hw/demo.dae";  // 更改为你的 DAE 文件路径

  // 读取DAE文件并加载
  shape_msgs::Mesh mesh;
  if (!moveit_commander::loadMeshFromFile(mesh_file, mesh)) {
    ROS_ERROR("Failed to load mesh file: %s", mesh_file.c_str());
    return -1;
  }

  // 创建一个障碍物的Pose
  geometry_msgs::Pose obstacle_pose;
  obstacle_pose.position.x = 1.0;  // 设置障碍物的位置
  obstacle_pose.position.y = 0.0;
  obstacle_pose.position.z = 0.5;
  obstacle_pose.orientation.w = 1.0;

  // 创建一个附加到规划场景的对象
  std::string mesh_id = "mesh_obstacle";
  planning_scene_interface.applyCollisionObject(mesh_id, mesh, obstacle_pose);

  // 发布 transform 到 base_link
  geometry_msgs::TransformStamped transformStamped;

  // 假设变换从 "mesh_frame" 到 "base_link"
  transformStamped.header.stamp = ros::Time::now();
  transformStamped.header.frame_id = "base_link";
  transformStamped.child_frame_id = "mesh_frame";

  // 设置物体相对于 base_link 的位置
  transformStamped.transform.translation.x = obstacle_pose.position.x;
  transformStamped.transform.translation.y = obstacle_pose.position.y;
  transformStamped.transform.translation.z = obstacle_pose.position.z;

  // 设置物体相对于 base_link 的姿态
  transformStamped.transform.rotation = obstacle_pose.orientation;

  // 广播变换
  tf_broadcaster.sendTransform(transformStamped);

  ROS_INFO("Mesh obstacle added to planning scene, transform broadcasted from base_link to mesh_frame.");

  // 等待并关闭ROS节点
  ros::waitForShutdown();
  return 0;
}

