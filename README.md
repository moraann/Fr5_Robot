Fr5_ROS包使用流程  Update 2025/1/6 21:40

该ROS包使用内部的Fr5机械臂模型开发，新增节点文件均位于catkin_ws/src/frcobot_ros/focobot_hw/src目录下.
使用前请在catkin_ws目录下执行 catkin_make clear 重新编译该ROS包

以下是ROS包使用方法，请顺序打开多个终端执行以下的命令行即可(为方便调试暂未整合成一个整体的launch文件)：
    // 启动rviz可视化界面、基础机械臂相关模型、moveit!规划器、机械臂状态同步等基础节点。启动之后可以看到rviz界面中下位机状态正确同步到rviz中
    1. roslaunch fr5_moveit_config demo_simulation.launch

    // 启动点云发布节点。启动之后会将指定的.ply点云文件发布到plyy frame下。（可手动添加pointcloud2对象进行可视化）
    2. rosrun frcobot_hw ply_tf_loader

    // 启动octomap体素栅格转换节点。启动后会将plyy frame下的点云进行指定分辨率的体素栅格化并发布到octomap_binary话题下
    3. rosrun frcobot_hw octomap_loader

    // 启动octomap到moveit碰撞场景发布节点。启动后rviz界面中将正确显示基于点云体素栅格化的碰撞体
    4. rosrun frcobot_hw otm_moveit

    // 启动机械臂样条运动监听节点。 启动后终端将持续返回机械臂关节运动状态
    5. rosrun frcobot_hw yt_run

    // 启动机械臂路径规划任务节点，完成该节点代码内指定的目标位置的样条运动的执行
    6. rosrun frcobot_hw plan


以下是ROS包各节点说明：（新增节点均位于frcobot_hw/src）
    ply_tf_loader：将PLY格式的3D点云读取并转化为TF坐标系并发布 
        在该文件中可调整要发布的障碍物.ply文件的路径

    octomap_loader：将octomap格式的地图数据转化为体素栅格并发布
        在该文件中可调整体素化栅格的分辨率大小

    otm_moveit：将octomap数据转换为moveit!碰撞场景并发布

    yt_run：机械臂样条运动监听节点，接收机械臂运动的命令并执行
        在该文件中可调整机械臂样条运动的各项机械参数

    plan：机械臂路径规划任务节点，完成运动的执行，并将执行结果返回到yt_run节点
        在该文件中可手动调整目标位姿的具体位置，当前默认为关节空间坐标系下的六个参数，如需改为笛卡尔空间下坐标指定，添加IK解算即可

    simulation_pos: 读取下位机（默认192.168.58.2）的关节状态并返回到rviz中

    ply_loader:  （已弃用，改为ply_tf_loader节点）读取并发布PLY格式的3D点云

    fake_state:  （调试用）在不连接下位机的情况下，模拟发布下位机关节状态

    frcobot_cmd_demo:  （已弃用）原ROS包调试文件

    frcobot_status_node:  （调试用）持续返回下位机机械臂关节状态

    obstacle: （已弃用）发布障碍物的测试节点



