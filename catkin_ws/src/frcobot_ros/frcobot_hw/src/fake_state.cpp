#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>


int main(int argc, char** argv)
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "frrobot_joint_state_publisher");
    ros::NodeHandle nh;

    // 创建发布者，发布到/joint_states话题
    ros::Publisher joint_state_pub = nh.advertise<sensor_msgs::JointState>("/joint_states", 10);

    // 设置关节名称，有6个关节
    std::vector<std::string> joint_names = {"j1", "j2", "j3", "j4", "j5", "j6"};

    ros::Rate loop_rate(50);  // 设置循环频率为10Hz

    while (ros::ok())  // 当ROS运行时保持循环
    {
        // 创建JointState消息对象
        sensor_msgs::JointState joint_state;
        joint_state.header.stamp = ros::Time::now();  // 设置消息的时间戳
        joint_state.name = joint_names;  // 关节名称
        joint_state.position.resize(joint_names.size());  // 调整位置数组大小

        // 填充关节位置数据
        joint_state.position[0] = 0.204388;
        joint_state.position[1] = -1.394153;
        joint_state.position[2] = 1.527484;
        joint_state.position[3] = -3.294379;
        joint_state.position[4] = -1.698781;
        joint_state.position[5] = 0.084073;

        // 发布JointState消息
        joint_state_pub.publish(joint_state);

        // 调用ros::spinOnce()处理所有回调
        ros::spinOnce();

        // 按照设定的频率休眠
        loop_rate.sleep();
    }

    return 0;
}