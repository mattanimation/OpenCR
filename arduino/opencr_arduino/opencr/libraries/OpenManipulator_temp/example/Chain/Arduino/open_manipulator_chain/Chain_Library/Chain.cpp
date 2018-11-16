﻿/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#include "../../include/Chain/om_chain.h"

OM_CHAIN::OM_CHAIN()
{}
OM_CHAIN::~OM_CHAIN()
{}

void OM_CHAIN::initManipulator(bool using_platform)
{
  platform_ = using_platform;
  ////////// manipulator parameter initialization

  addWorld(WORLD, // world name
           COMP1);// child name

  addComponent(COMP1, // my name
               WORLD, // parent name
               COMP2, // child name
               RM_MATH::makeVector3(-0.278, 0.0, 0.017), // relative position
               RM_MATH::convertRPYToRotation(0.0, 0.0, 0.0), // relative orientation
               Z_AXIS, // axis of rotation
               11); // actuator id

  addComponent(COMP2, // my name
               COMP1, // parent name
               COMP3, // child name
               RM_MATH::makeVector3(0.0, 0.0, 0.058), // relative position
               RM_MATH::convertRPYToRotation(0.0, 0.0, 0.0), // relative orientation
               Y_AXIS, // axis of rotation
               12); // actuator id

  addComponent(COMP3, // my name
               COMP2, // parent name
               COMP4, // child name
               RM_MATH::makeVector3(0.024, 0.0, 0.128), // relative position
               RM_MATH::convertRPYToRotation(0.0, 0.0, 0.0), // relative orientation
               Y_AXIS, // axis of rotation
               13); // actuator id

  addComponent(COMP4, // my name
               COMP3, // parent name
               TOOL, // child name
               RM_MATH::makeVector3(0.124, 0.0, 0.0), // relative position
               RM_MATH::convertRPYToRotation(0.0, 0.0, 0.0), // relative orientation
               Y_AXIS, // axis of rotation
               14); // actuator id

  addTool(TOOL, // my name
          COMP4, // parent name
          RM_MATH::makeVector3(0.130, 0.0, 0.0), // relative position
          RM_MATH::convertRPYToRotation(0.0, 0.0, 0.0), // relative orientation
          15, // actuator id
          1.0f); // Change unit from `meter` to `radian`

  ////////// kinematics init.
  kinematics_ = new OM_KINEMATICS::Chain();
  addKinematics(kinematics_);

  ////////// drawing path
  addDrawingTrajectory(DRAWING_LINE, &line_);
  addDrawingTrajectory(DRAWING_CIRCLE, &circle_);
  addDrawingTrajectory(DRAWING_RHOMBUS, &rhombus_);
  addDrawingTrajectory(DRAWING_HEART, &heart_);

  if(platform_)
  {
    ////////// joint actuator init.
    actuator_ = new OM_DYNAMIXEL::JointDynamixel();
    // communication setting argument
    String dxl_comm_arg = "1000000";
    void *p_dxl_comm_arg = &dxl_comm_arg;

    // set joint actuator id
    jointDxlId.push_back(11);
    jointDxlId.push_back(12);
    jointDxlId.push_back(13);
    jointDxlId.push_back(14);

    addJointActuator(JOINT_DYNAMIXEL, actuator_, jointDxlId, p_dxl_comm_arg);

    // set joint actuator control mode
    String joint_dxl_mode_arg = "position_mode";
    void *p_joint_dxl_mode_arg = &joint_dxl_mode_arg;
    jointActuatorSetMode(JOINT_DYNAMIXEL, jointDxlId, p_joint_dxl_mode_arg);

    String joint_dxl_opt_arg[2] = {"Return_Delay_Time", "0"};
    void *p_joint_dxl_opt_arg = &joint_dxl_opt_arg;
    jointActuatorSetMode(JOINT_DYNAMIXEL, jointDxlId, p_joint_dxl_opt_arg);

    ////////// tool actuator init.
    tool_ = new OM_DYNAMIXEL::GripperDynamixel();

    uint8_t gripperDxlId = 15;
    addToolActuator(TOOL_DYNAMIXEL, tool_, gripperDxlId, p_dxl_comm_arg);

    // set gripper actuator control mode
    String gripper_dxl_mode_arg = "current_based_position_mode";
    void *p_gripper_dxl_mode_arg = &gripper_dxl_mode_arg;
    toolActuatorSetMode(TOOL_DYNAMIXEL, p_gripper_dxl_mode_arg);

    String gripper_dxl_opt_arg[2] = {"Profile_Velocity", "200"};
    void *p_gripper_dxl_opt_arg = &gripper_dxl_opt_arg;
    toolActuatorSetMode(TOOL_DYNAMIXEL, p_gripper_dxl_opt_arg);

    gripper_dxl_opt_arg[0] = "Profile_Acceleration";
    gripper_dxl_opt_arg[1] = "20";
    toolActuatorSetMode(TOOL_DYNAMIXEL, p_gripper_dxl_opt_arg);

    gripper_dxl_opt_arg[0] = "Return_Delay_Time";
    gripper_dxl_opt_arg[1] = "0";
    toolActuatorSetMode(TOOL_DYNAMIXEL, p_gripper_dxl_opt_arg);
    
    // all actuator enable
    allActuatorEnable();
    receiveAllJointActuatorValue();
    receiveToolActuatorValue(TOOL);
  }
  
  ////////// manipulator trajectory & control time initialization
  initTrajectoryWayPoint();
  setControlTime(CONTROL_TIME);
}

void OM_CHAIN::chainProcess(double present_time)
{
  
  std::vector<WayPoint> goal_value = trajectoryControllerLoop(present_time);

  if(platform_)
  {
    receiveAllJointActuatorValue();
    receiveToolActuatorValue(TOOL);
    if(goal_value.size() != 0)  sendAllJointActuatorValue(goal_value);
    forward();
  }
  else
  {
    if(goal_value.size() != 0) setAllActiveJointValue(goal_value); // visualization
    forward();
  } 
}

bool OM_CHAIN::getPlatformFlag()
{
  return platform_;
}
