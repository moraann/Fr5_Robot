#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "FRRobot.h"
#include "RobotTypes.h"

int main(int argc, char** argv)
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "frrobot_joint_state_publisher");
    ros::NodeHandle nh;

    // 创建发布者，发布到/joint_states话题
    ros::Publisher joint_state_pub = nh.advertise<sensor_msgs::JointState>("/joint_states", 10);

    // 创建一个JointPos结构体用于存储机械臂的实际关节角度
    JointPos j_rad;
    int flag = 1;
    memset(&j_rad, 0, sizeof(JointPos));

    // 创建FRRobot对象并连接到机器人
    FRRobot robot;
    robot.RPC("192.168.58.2");

    // 设置关节名称，假设有6个关节
    std::vector<std::string> joint_names = {"j1", "j2", "j3", "j4", "j5", "j6"};

    ros::Rate loop_rate(50);  // 设置循环频率为10Hz

    while (ros::ok())  // 当ROS运行时保持循环
    {
        // 获取机械臂的实际关节弧度
        robot.GetActualJointPosRadian(flag, &j_rad);
        printf("joint pos rad:%f,%f,%f,%f,%f,%f\n", j_rad.jPos[0],j_rad.jPos[1],j_rad.jPos[2],j_rad.jPos[3],j_rad.jPos[4],j_rad.jPos[5]);
        // 创建JointState消息对象
        sensor_msgs::JointState joint_state;
        joint_state.header.stamp = ros::Time::now();  // 设置消息的时间戳
        joint_state.name = joint_names;  // 关节名称
        joint_state.position.resize(joint_names.size());  // 调整位置数组大小

        // 填充关节位置数据
        for (size_t i = 0; i < joint_names.size(); ++i) {
            joint_state.position[i] = j_rad.jPos[i];  // 获取机械臂的实际角度
        }

        // 发布JointState消息
        joint_state_pub.publish(joint_state);

        // 调用ros::spinOnce()处理所有回调
        ros::spinOnce();

        // 按照设定的频率休眠
        loop_rate.sleep();
    }

    return 0;
}
