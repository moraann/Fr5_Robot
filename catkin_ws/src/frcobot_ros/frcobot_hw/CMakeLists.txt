cmake_minimum_required(VERSION 3.0.2)
project(frcobot_hw)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(octomap REQUIRED)
find_package(octomap_msgs REQUIRED)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  moveit_ros_planning_interface
  moveit_commander
  moveit_core
  moveit_ros_planning
  moveit_ros_visualization
  pcl_ros
  pcl_conversions
  sensor_msgs
  geometry_msgs
  shape_msgs
  tf2_ros
  
  #Eigen3
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  status.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  #INCLUDE_DIRS include
#  LIBRARIES frcobot_hw
 CATKIN_DEPENDS roscpp rospy std_msgs message_runtime geometry_msgs 
 #moveit_commander moveit_ros_planning_interface shape_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  include/frcobot_hw
  include/xmlrpc-c
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${OCTOMAP_INCLUDE_DIRS}
)

link_directories(
  lib
  ${catkin_LIB_DIRS}
)

set(FRCOBOT_CMD src/frcobot_cmd_demo.cpp)

set(FRCOBOT_STATUS src/frcobot_status_node.cpp)

set(SIMULATION_POS src/simulation_pos.cpp)


set(PLAN src/plan.cpp)

set(YT_RUN src/yt_run.cpp)

set(PLY_LOADER src/ply_loader.cpp)

set(OCTOMAP_LOADER src/octomap_loader.cpp)

set(OBSTACLE src/obstacle.cpp)

set(OTM_MOVEIT src/otm_moveit.cpp)

set(PLY_TF_LOADER src/ply_tf_loader.cpp)

set(FAKE_STATE src/fake_state.cpp)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/frcobot_hw.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/frcobot_hw_node.cpp)
add_executable(frcobot_cmd_demo  ${FRCOBOT_CMD})
target_link_libraries(frcobot_cmd_demo  ${catkin_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(frcobot_status_node ${FRCOBOT_STATUS})
add_dependencies(frcobot_status_node ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(frcobot_status_node  ${catkin_LIBRARIES})

add_executable(simulation_pos  ${SIMULATION_POS})
target_link_libraries(simulation_pos  ${catkin_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(fake_state  ${FAKE_STATE})
target_link_libraries(fake_state  ${catkin_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)



add_executable(plan  ${PLAN})
target_link_libraries(plan  ${catkin_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(yt_run  ${YT_RUN})
target_link_libraries(yt_run  ${catkin_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(ply_loader  ${PLY_LOADER})
target_link_libraries(ply_loader  ${catkin_LIBRARIES}
  ${PLY_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(octomap_loader  ${OCTOMAP_LOADER})
target_link_libraries(octomap_loader  ${catkin_LIBRARIES}
  ${PLY_LIBRARIES}
  ${OCTOMAP_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(otm_moveit  ${OTM_MOVEIT})
target_link_libraries(otm_moveit  ${catkin_LIBRARIES}
  ${PLY_LIBRARIES}
  ${OCTOMAP_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

add_executable(ply_tf_loader  ${PLY_TF_LOADER})
target_link_libraries(ply_tf_loader  ${catkin_LIBRARIES}
  ${PLY_LIBRARIES}
  ${OCTOMAP_LIBRARIES}
  robotAPI
  xmlrpc
  xmlrpc_client
  xmlrpc_util
  xmlrpc_xmlparse
  xmlrpc_xmltok
  z
)

#add_executable(obstacle  ${OBSTACLE})
#target_link_libraries(obstacle  ${catkin_LIBRARIES}
# ${PLY_LIBRARIES}
 # ${OCTOMAP_LIBRARIES}
 # robotAPI
 # xmlrpc
 # xmlrpc_client
 # xmlrpc_util
  #xmlrpc_xmlparse
 # xmlrpc_xmltok
 # z
#)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_frcobot_hw.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
