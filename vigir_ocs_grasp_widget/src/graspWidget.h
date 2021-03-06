/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013-2015, Team ViGIR ( TORC Robotics LLC, TU Darmstadt, Virginia Tech, Oregon State University, Cornell University, and Leibniz University Hanover )
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Team ViGIR, TORC Robotics, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/
//@TODO_ADD_AUTHOR_INFO
#ifndef GRASPWIDGET_H
#define GRASPWIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>
#include <QPainter>
#include <QtGui>

#include <vector>
#include <algorithm>

#include <ros/ros.h>

#include "tf/transform_listener.h"
#include <tf_conversions/tf_eigen.h>

#include "handOffsetWidget.h"

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>

#include <sensor_msgs/JointState.h>

#include <moveit_msgs/RobotState.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

#include <vigir_ocs_msgs/OCSTemplateList.h>
#include <vigir_ocs_msgs/OCSTemplateRemove.h>
#include <vigir_ocs_msgs/OCSRobotStatus.h>
#include <vigir_ocs_msgs/RobotStatusCodes.h>
#include <vigir_ocs_msgs/OCSLinkColor.h>
#include <vigir_ocs_msgs/OCSKeyEvent.h>
#include <vigir_ocs_msgs/OCSObjectSelection.h>
#include <vigir_ocs_msgs/OCSGraspSync.h>
#include <vigir_grasp_msgs/GraspState.h>
#include <vigir_grasp_msgs/GraspSelection.h>
#include <vigir_grasp_msgs/TemplateSelection.h>
#include <vigir_grasp_msgs/LinkState.h>

#include <vigir_object_template_msgs/GetGraspInfo.h>
#include <vigir_object_template_msgs/GetTemplateStateAndTypeInfo.h>

#define FINGER_EFFORTS 4

namespace Ui {
class graspWidget;
}

class graspWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit graspWidget(QWidget *parent = 0, std::string hand = "left", std::string hand_name = "l_hand");
    ~graspWidget();

    void processObjectSelection(const vigir_ocs_msgs::OCSObjectSelection::ConstPtr msg);
    //void processNewKeyEvent(const vigir_ocs_msgs::OCSKeyEvent::ConstPtr& pose);

    Ui::graspWidget * getUi();
    QLayout * getMainLayout();
public Q_SLOTS:
    void on_userSlider_sliderReleased();
    void on_templateBox_activated(const QString &arg1);
    void on_graspBox_activated(const QString &arg1);
    void on_affordanceBox_activated(const int &arg1);
    void on_usabilityBox_activated(const int &arg1);
    void on_performButton_clicked();
    void on_moveToPoseButton_clicked();
    void on_releaseButton_clicked();
    void on_show_grasp_toggled(bool checked);
    void on_attachButton_clicked();
    void on_detachButton_clicked();
    void on_affordanceButton_clicked();
    void on_displacementBox_valueChanged(double value);
    void on_pitchSpinBox_valueChanged(double value);
    void on_keepOrientationBox_toggled(bool checked);
    void on_snapTemplateButton_clicked();
    void on_graspPose_clicked();



private:
    void sendManualMsg(uint8_t level);

    QString icon_path_;
    void setUpButtons();

    Ui::graspWidget *ui;
    handOffsetWidget *ui2;

    void graspStateReceived (const vigir_grasp_msgs::GraspState::ConstPtr graspState);
    void graspSelectedReceived (const vigir_grasp_msgs::GraspSelection::ConstPtr graspMsg);
    void processTemplateList( const vigir_ocs_msgs::OCSTemplateList::ConstPtr list);
    void templateMatchFeedback (const vigir_grasp_msgs::TemplateSelection::ConstPtr feedback);
    void processGraspSyncCB(const vigir_ocs_msgs::OCSGraspSync::ConstPtr msg);

    int calcWristTarget( const geometry_msgs::Pose& wrist_pose,const geometry_msgs::PoseStamped& template_pose, geometry_msgs::PoseStamped& final_pose );
    void calcPlanningTarget(const geometry_msgs::Pose& palm_pose, const geometry_msgs::PoseStamped& template_pose, geometry_msgs::PoseStamped& planning_hand_pose);
    int hideHand();
    int staticTransform(geometry_msgs::Pose& palm_pose);
    int poseTransform(geometry_msgs::Pose& first_pose, geometry_msgs::Pose& second_pose);
    int poseTransform(geometry_msgs::Pose& input_pose, tf::Transform transform);
    void gripperTranslationToPreGraspPose(geometry_msgs::Pose& pose, moveit_msgs::GripperTranslation& trans);


    // need to store updated template list and selected template id to calculate final position of the hand
    vigir_object_template_msgs::GetGraspInfo                last_grasp_srv_;
    vigir_object_template_msgs::GetTemplateStateAndTypeInfo last_template_srv_;

    vigir_ocs_msgs::OCSTemplateList         last_template_list_;
    geometry_msgs::PoseStamped             frameid_T_template_;
    vigir_object_template_msgs::Affordance current_affordance_;
    int selected_template_id_;
    int selected_grasp_id_;
    int selected_affordance_id_;
    int selected_usability_id_;

    ros::NodeHandle nh_;
    ros::Subscriber grasp_state_sub_;
    ros::Subscriber grasp_selected_sub_;
    ros::Subscriber template_select_sub;
    ros::Subscriber template_list_sub_;
    ros::Subscriber template_match_feedback_sub_;
    ros::Subscriber grasp_sync_sub_;
    ros::Publisher grasp_selection_pub_;
    ros::Publisher template_remove_pub_;
    ros::Publisher grasp_command_pub_;
    ros::Publisher template_match_request_pub_;
    ros::Publisher move_request_pub_;
    ros::Publisher detach_object_pub_;
    ros::Publisher template_stitch_request_pub_;
    ros::Publisher grasp_release_pub_;
    ros::Publisher affordance_selection_pub_;
    ros::Publisher snap_template_pub_;
    ros::Publisher hand_marker_pub_;
    ros::Publisher grasp_sync_pub_;

    ros::Publisher planning_hand_target_pub_;

    geometry_msgs::Pose feedbackPose;

    bool templateMatchDone;
    bool follow_ban_;

    std::string hand_side_;
    std::string hand_type_;
    std::string hand_name_;
    std::string palm_link_;
    std::string wrist_link_;
    std::string hand_group_;
    uint8_t currentGraspMode;

    std::string operator_type_;

    std::string left_wrist_link_, right_wrist_link_;
    std::string left_palm_link_,  right_palm_link_;
    std::string left_hand_group_, right_hand_group_;

    // **************************
    // show robot status messages
    ros::Subscriber robot_status_sub_;
    ros::Subscriber template_stitch_pose_sub_;
    RobotStatusCodes robot_status_codes_;

    void robotStatusCB(const vigir_ocs_msgs::OCSRobotStatus::ConstPtr msg);
    void templateStitchPoseCallback(const geometry_msgs::PoseStamped::ConstPtr msg);

    // publisher to color fingers/hand
    ros::Publisher hand_link_color_pub_;
    void publishLinkColor(std::string, unsigned char r, unsigned char g, unsigned char b);

    // publisher for ghost hand position
    ros::Publisher ghost_hand_pub_;
    void publishHandPose(unsigned int id);

    // publisher for the finger joints
    ros::Publisher ghost_hand_joint_state_pub_;
    void publishHandJointStates(std::vector<float>&, std::vector<std::string> &finger_joint_names);

    tf::TransformListener tf_;

    tf::Transform stitch_template_pose_;
    tf::Transform hand_offset_pose_;
    tf::Transform hand_T_palm_;   //describes palm in hand frame
    tf::Transform gp_T_palm_;     //describes palm in grasp pose frame

    // get joint states
    ros::Subscriber link_states_sub_;

    void linkStatesCB(const vigir_grasp_msgs::LinkState::ConstPtr link_states);

    bool show_grasp_;

    robot_model_loader::RobotModelLoaderPtr hand_model_loader_;
    robot_model::RobotModelPtr              hand_robot_model_;
    robot_state::RobotStatePtr              hand_robot_state_;
    robot_model_loader::RobotModelLoaderPtr robot_model_loader_;
    robot_model::RobotModelPtr              robot_model_;

    std::vector<std::string>   hand_joint_names_;

    moveit_msgs::DisplayRobotState display_state_msg_;
    ros::Publisher robot_state_vis_pub_;

    // Used to make setting virtual joint positions (-> hand pose) easier
    sensor_msgs::JointState virtual_link_joint_states_;

    ros::Publisher     select_object_pub_;
    ros::Subscriber    select_object_sub_;

    std::vector<int>   keys_pressed_list_;

    ros::Subscriber    key_event_sub_;

    ros::ServiceClient grasp_info_client_;
    ros::ServiceClient template_info_client_;

protected:
    void timerEvent(QTimerEvent *event);


private:
    QBasicTimer timer;
};

#endif // GRASPWIDGET_H
