/**
 * *********************************************************
 *
 * @file: sample_planner.h
 * @brief: Contains the sample planner ROS wrapper class
 * @author: Yang Haodong
 * @date: 2022-10-26
 * @version: 1.0
 *
 * Copyright (c) 2024, Yang Haodong.
 * All rights reserved.
 *
 * --------------------------------------------------------
 *
 * ********************************************************
 */
#ifndef SAMPLE_PLANNER_H
#define SAMPLE_PLANNER_H

#include <nav_core/base_global_planner.h>
#include <nav_msgs/Path.h>
#include <nav_msgs/GetPlan.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/ColorRGBA.h>
#include <std_msgs/Header.h>
#include <visualization_msgs/Marker.h>

#include "global_planner.h"

namespace sample_planner
{
class SamplePlanner : public nav_core::BaseGlobalPlanner
{
public:
  /**
   * @brief  Constructor(default)
   */
  SamplePlanner();

  /**
   * @brief  Constructor
   * @param  name     planner name
   * @param  costmap  costmap pointer
   * @param  frame_id costmap frame ID
   */
  SamplePlanner(std::string name, costmap_2d::Costmap2D* costmap, std::string frame_id);

  /**
   * @brief Destructor
   * @return No return value
   * @details default
   */
  ~SamplePlanner() = default;

  /**
   * @brief  Planner initialization
   * @param  name         planner name
   * @param  costmapRos   costmap ROS wrapper
   */
  void initialize(std::string name, costmap_2d::Costmap2DROS* costmapRos);

  /**
   * @brief  Planner initialization
   * @param  name     planner name
   * @param  costmap  costmap pointer
   * @param  frame_id costmap frame ID
   */
  void initialize(std::string name, costmap_2d::Costmap2D* costmap, std::string frame_id);

  /**
   * @brief plan a path given start and goal in world map
   * @param start     start in world map
   * @param goal      goal in world map
   * @param plan      plan
   * @param tolerance error tolerance
   * @return true if find a path successfully else false
   */
  bool makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal,
                std::vector<geometry_msgs::PoseStamped>& plan);
  bool makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal, double tolerance,
                std::vector<geometry_msgs::PoseStamped>& plan);

  /**
   * @brief  publish planning path
   * @param  path planning path
   */
  void publishPlan(const std::vector<geometry_msgs::PoseStamped>& plan);

  /**
   * @brief  regeister planning service
   * @param  req  request from client
   * @param  resp response from server
   */
  bool makePlanService(nav_msgs::GetPlan::Request& req, nav_msgs::GetPlan::Response& resp);

protected:
  /**
   * @brief  publish expand zone
   * @param  expand  set of expand nodes
   */
  void _publishExpand(std::vector<Node>& expand);

  /**
   * @brief  calculate plan from planning path
   * @param  path path generated by global planner
   * @param  plan plan transfromed from path
   * @return bool true if successful else false
   */
  bool _getPlanFromPath(std::vector<Node> path, std::vector<geometry_msgs::PoseStamped>& plan);

  /**
   *  @brief Publishes a Marker msg with two points in Rviz
   *  @param line_msg Pointer to existing marker object.
   *  @param line_pub Pointer to existing marker Publisher.
   *  @param id first marker id
   *  @param pid second marker id
   */
  void _pubLine(visualization_msgs::Marker* line_msg, ros::Publisher* line_pub, int id, int pid);

protected:
  std::string frame_id_;                                      // costmap frame ID
  ros::Publisher plan_pub_;                                   // path planning publisher
  bool initialized_;                                          // initialization flag
  std::shared_ptr<global_planner::GlobalPlanner> g_planner_;  // global graph planner
  ros::Publisher expand_pub_;                                 // nodes explorer publisher
  ros::ServiceServer make_plan_srv_;                          // planning service

private:
  double tolerance_;                                      // tolerance
  bool is_outline_;                                       // whether outline the boudary of map
  double factor_;                                         // obstacle inflation factor
  bool is_expand_;                                        // whether publish expand map or not
  int sample_points_;                                     // random sample points
  double sample_max_d_;                                   // max distance between sample points
  double opt_r_;                                          // optimization raidus
  double prior_set_r_;                                    // radius of priority sample circles
  int rewire_threads_n_;                                  // threads number of rewire process
  double step_ext_d_;                                     // increased distance of adaptive extend step size
  double t_freedom_;                                      // freedom of t distribution
  std::vector<geometry_msgs::PoseStamped> history_plan_;  // history plan
};
}  // namespace sample_planner
#endif