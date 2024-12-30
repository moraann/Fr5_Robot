ROS包使用流程  Update 2024/12/17 17:09

// 启动rviz可视化界面、基础机械臂相关模型、moveit!规划器、机械臂状态同步等基础节点
1. roslaunch fr5_moveit_config demo_simulation.launch

// 启动点云发布节点
2. rosrun frcobot_hw ply_tf_loader

// 启动octomap体素栅格转换节点
3. rosrun frcobot_hw octomap_loader

// 启动octomap到moveit碰撞场景发布节点
4. rosrun frcobot_hw otm_moveit

// 启动机械臂样条运动监听节点
5. rosrun frcobot_hw yt_run

// 启动机械臂路径规划任务节点，完成运动的执行
6. rosrun frcobot_hw plan



