#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_interface/planning_interface.h>
#include <moveit/robot_trajectory/robot_trajectory.h>
#include <ros/ros.h>
#include <cmath>  // 包含 M_PI
#include <std_msgs/Float64MultiArray.h>  // 用于发布关节位置的消息类型
#include <moveit_msgs/RobotTrajectory.h>  // 使用 moveit_msgs::RobotTrajectory

int main(int argc, char** argv)
{
  ros::init(argc, argv, "moveit_planner");
  if (!ros::ok()) {
    ROS_ERROR("ROS initialization failed");
    return 1;
  }
  ROS_INFO("ROS initialization succeeded");

  ros::AsyncSpinner async_spinner(2);
  async_spinner.start();

  ros::NodeHandle node_handle;

  // 初始化 MoveGroupInterface，参数为机械臂的规划组名称
  moveit::planning_interface::MoveGroupInterface move_group("fr5_arm");

  // 定义目标关节角度
  std::vector<double> joint_group_angles_degrees;
  joint_group_angles_degrees.push_back(0.0);  // joint_1 
  joint_group_angles_degrees.push_back(2.5);  // joint_2 
  joint_group_angles_degrees.push_back(22.6);  // joint_3 
  joint_group_angles_degrees.push_back(-207.4);  // joint_4 
  joint_group_angles_degrees.push_back(-88.6); // joint_5 
  joint_group_angles_degrees.push_back(0.2); // joint_6 

  // 将角度转换为弧度
  std::vector<double> joint_group_positions;
  for (double angle : joint_group_angles_degrees)
  {
    joint_group_positions.push_back(angle * M_PI / 180.0);  // 转换为弧度
  }

  // 设置目标位置
  move_group.setJointValueTarget(joint_group_positions);
  move_group.setPlannerId("RRT");
  // 调用路径规划
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  ROS_INFO("Ready to planning...");
  //moveit::core::MoveItErrorCode planning_result = move_group.plan(my_plan);  // 修改为 MoveItErrorCode 类型
  bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  //move_group.asyncMove();  // 异步执行，允许主线程继续运行
  ROS_INFO("Planning has finished");
  //if (planning_result == moveit::core::MoveItErrorCode::SUCCESS)  // 使用 MoveItErrorCode 进行比较
  if(success)
  {
    ROS_INFO("Plan success!");
  }
  else
  {
    ROS_INFO("Plan failed!");  
    return 1;
  }
  //ROS_INFO("路径规划状态验证结束！");
  // 获取规划的轨迹
  moveit_msgs::RobotTrajectory trajectory = my_plan.trajectory_;  // 使用 moveit_msgs::RobotTrajectory 类型

  // 打印轨迹信息（可选）
  ROS_INFO_STREAM("Count of Points " << trajectory.joint_trajectory.points.size());

  // 创建 ROS Publisher，用于发布关节角度
  ros::Publisher joint_pub = node_handle.advertise<std_msgs::Float64MultiArray>("joint_angles", 10);

  // 获取轨迹中的每个点，转换成角度并发布
  size_t num_points = trajectory.joint_trajectory.points.size();
  
  // 设置时间间隔，这里假设你可以从轨迹中获取时间信息
  ros::Time last_publish_time = ros::Time::now();

  for (size_t i = 0; i < num_points; ++i)
  {
    const trajectory_msgs::JointTrajectoryPoint& point = trajectory.joint_trajectory.points[i];
    
    // 使用 getJointPositions() 获取关节位置
    std::vector<double> joint_positions;
    for (size_t j = 0; j < point.positions.size(); ++j)
    {
      joint_positions.push_back(point.positions[j]);
    }

    // 转换关节角度（弧度 -> 角度）
    std::vector<double> joint_rad;
    for (size_t j = 0; j < joint_positions.size(); ++j)
    {
      double angle_in_degrees = joint_positions[j] * 180.0 / M_PI;
      joint_rad.push_back(angle_in_degrees);
    }

    joint_rad.push_back((float)num_points);

    // 创建消息并发布
    std_msgs::Float64MultiArray joint_msg;
    joint_msg.data = joint_rad;
    joint_pub.publish(joint_msg);

    // 延时一定时间以确保消息有时间发送
    // 这里假设每个点之间的时间间隔为 0.1s（可以根据实际需要调整）
    ros::Duration(0.2).sleep();  // 你可以根据需要进一步调整这个间隔
  }

  ros::shutdown();
  return 0;
}

