#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include "FRRobot.h"
#include "RobotTypes.h"

using namespace std;

FRRobot robot;  // 实例化机器人对象
//是否需要转换样条运动状态
bool moveflag = false;
// 定义回调函数，用来接收关节角度消息并执行样条运动
void jointAnglesCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    ROS_INFO("New Point!");

    // 目标关节位置
    JointPos target_joint_pos;
    memset(&target_joint_pos, 0, sizeof(JointPos));

    // 将接收到的角度（度）转换为目标关节角度
    for (size_t i = 0; i < 6; ++i)
    {
        target_joint_pos.jPos[i] = msg->data[i];  // 将 ROS 消息中的角度数据填充到目标关节数据中
    }

    // 获取当前关节的实际角度
    JointPos current_joint_pos;
    robot.GetActualJointPosDegree(1, &current_joint_pos);  // 获取当前关节角度，单位是度

    // 使用正向运动学（FK）计算当前关节角度对应的笛卡尔位姿
    DescPose current_desc_pose;
    int result = robot.GetForwardKin(&current_joint_pos, &current_desc_pose); // 通过正向运动学计算当前笛卡尔位姿

    if (result != 0) {
        ROS_ERROR("Failed to calculate Cartesian pose for current joint angles");
        return;
    }

    // 打印当前的笛卡尔位姿
    ROS_INFO("Current Cartesian Pose: x=%.3f, y=%.3f, z=%.3f, rx=%.3f, ry=%.3f, rz=%.3f",
             current_desc_pose.tran.x, current_desc_pose.tran.y, current_desc_pose.tran.z,
             current_desc_pose.rpy.rx, current_desc_pose.rpy.ry, current_desc_pose.rpy.rz);

    // 使用正向运动学（FK）计算目标关节角度对应的笛卡尔位姿
    DescPose target_desc_pose;
    result = robot.GetForwardKin(&target_joint_pos, &target_desc_pose);  // 通过目标关节角度计算目标笛卡尔位姿

    if (result != 0) {
        ROS_ERROR("Failed to calculate Cartesian pose for target joint angles");
        return;
    }

    // 打印目标的笛卡尔位姿
    ROS_INFO("Target Cartesian Pose: x=%.3f, y=%.3f, z=%.3f, rx=%.3f, ry=%.3f, rz=%.3f",
             target_desc_pose.tran.x, target_desc_pose.tran.y, target_desc_pose.tran.z,
             target_desc_pose.rpy.rx, target_desc_pose.rpy.ry, target_desc_pose.rpy.rz);

    // 现在可以使用关节角度和笛卡尔位姿来进行样条运动
    if(!moveflag)
    {
        ROS_ERROR("Spline Start!");
        
        //开始前停止点动，关闭残留的关节速度
        robot.StartJOG(0,1,0,0.1,0.1,0.2);
        robot.ImmStopJOG();

        robot.SplineStart();  // 开始样条运动
        moveflag = true;
    }
    ROS_INFO("Spline Doing! ");
    // 调用 SplinePTP 函数进行样条点到点运动
    result = robot.SplinePTP(&target_joint_pos, &target_desc_pose, 2, 0, 80, 100, 80);
    if (result != 0) {
        ROS_ERROR("SplinePTP failed with error code: %d", result);
    }
    //ros::Duration(0.1).sleep();
    //robot.GetRobotMotionDone(&ismove);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "point_tun");
    ros::NodeHandle node_handle;
    ros::Rate loop_rate(40);  // 设置循环频率为40Hz

    // 与机器人控制器建立通信连接
    robot.RPC("192.168.58.2");

    // 订阅关节角度消息
    ros::Subscriber sub = node_handle.subscribe("joint_angles", 0, jointAnglesCallback);
    // 检查机械臂运动状态
    uint8_t ismove = 0; 

    while (ros::ok()) {
        ROS_INFO("Listening...");
        robot.GetRobotMotionDone(&ismove);
        ROS_INFO("State:%d",ismove);
        if(moveflag && ismove)
        {
            ros::Duration(0.5).sleep(); //防订阅频率抖动
            robot.GetRobotMotionDone(&ismove);
            if(moveflag && ismove)
            {
                ROS_ERROR("Spline End");
                robot.SplineEnd();  // 结束样条运动
                moveflag = false;
            }
        }
        ros::spinOnce();  // 处理回调
        loop_rate.sleep();  // 按照设定的频率休眠
    }

    return 0;
}

