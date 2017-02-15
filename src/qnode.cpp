/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
 ** Includes
 *****************************************************************************/
#include "../include/controller_switcher/qnode.hpp"
#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include <controller_manager_msgs/ListControllers.h>
#include <controller_manager_msgs/SwitchController.h>
#include <controller_manager_msgs/ControllerState.h>
#include <lwr_force_position_controllers/FtSensorInit.h>

/*****************************************************************************
 ** Namespaces
 *****************************************************************************/

namespace controller_switcher {

  /*****************************************************************************
   ** Implementation
   *****************************************************************************/

  QNode::QNode(int argc, char** argv ) :
    init_argc(argc),
    init_argv(argv)
  {}

  QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    wait();
  }

  bool QNode::init() {
    ros::init(init_argc,init_argv,"controller_switcher");
    if ( ! ros::master::check() ) {
      return false;
    }
    ros::start();
    start();

    // sleep so that controller spawner can load all the controllers
    ros::Duration(3).sleep();

    return true;
  }

  bool QNode::get_controllers_list(std::vector<std::string>& running_list, std::vector<std::string>& stopped_list)
  {
    ros::NodeHandle n;
    ros::ServiceClient client;
    controller_manager_msgs::ListControllers service;
    std::vector<controller_manager_msgs::ControllerState> controller_list;

    ros::service::waitForService("/" + robot_namespace_ + "/controller_manager/list_controllers");

    client = n.serviceClient<controller_manager_msgs::ListControllers>("/" + robot_namespace_ + "/controller_manager/list_controllers");
    if(!client.call(service))
      return false;
    controller_list = service.response.controller;

    for (std::vector<controller_manager_msgs::ControllerState>::iterator it = controller_list.begin();
	 it != controller_list.end(); ++it)
      {
	if(it->state == "running")
	  running_list.push_back(it->name);
	else if (it->state == "stopped")
	  stopped_list.push_back(it->name);
      }

    return true;
  }

  bool QNode::switch_controllers(const std::string start_controller, const std::string stop_controller)
  {
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<controller_manager_msgs::SwitchController>("/" + robot_namespace_ + "/controller_manager/switch_controller");
    controller_manager_msgs::SwitchController service;
    std::vector<std::string> start_controllers;
    std::vector<std::string> stop_controllers;
    start_controllers.push_back(start_controller);
    stop_controllers.push_back(stop_controller);
    service.request.start_controllers = start_controllers;
    service.request.stop_controllers = stop_controllers;
    service.request.strictness = 2;

    if(!client.call(service))
      return false;

    return true;

  }

  void QNode::set_robot_namespace(std::string name)
  {
    robot_namespace_ = name;
  }

  bool QNode::set_ftsensor(lwr_force_position_controllers::FtSensorInitMsg& response)
  {
    ros::NodeHandle n;
    ros::ServiceClient client;
    lwr_force_position_controllers::FtSensorInit service;
    bool outcome;

    client = n.serviceClient<lwr_force_position_controllers::FtSensorInit>("/" + robot_namespace_ + "/ft_sensor_controller/sensor_ctl_init");

    outcome = client.call(service);
    
    if (outcome)
      response = service.response.message;
    
    return outcome;
  }

  bool QNode::get_ftsensor_config(lwr_force_position_controllers::FtSensorInitMsg& response)
  {
    // FIXME:
    // add a field in FtSensorInitMsg so that we can use one service to get the current
    // configuration AND (if needed) update it using new sensor data
    //

    ros::NodeHandle n;
    ros::ServiceClient client;
    lwr_force_position_controllers::FtSensorInit service;
    bool outcome;

    client = n.serviceClient<lwr_force_position_controllers::FtSensorInit>("/" + robot_namespace_ + "/ft_sensor_controller/get_sensor_config");

    outcome = client.call(service);
    
    if (outcome)
      response = service.response.message;
    
    return outcome;

  }  

  void QNode::run() {
    // ros::Rate loop_rate(1);
    // while ( ros::ok() ) {
    // 	ros::spinOnce();
    // 	loop_rate.sleep();
    // }
    // Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
  }

}  // namespace controller_switcher
