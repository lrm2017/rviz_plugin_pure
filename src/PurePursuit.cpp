//
// Created by lrm on 22-11-21.
//

#include <stdio.h>
#include <QPainter>
#include <QLineEdit>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

#include <ackermann_msgs/AckermannDrive.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>

#include "PurePursuit.h"
#include "drivewidget.h"

namespace pure_rviz_plugin{

PurePursuit::PurePursuit(QWidget *parent)
    : rviz::Panel(parent)
    , linear_vel(0), angular_vel(0)
{
    QHBoxLayout* topic_layout = new QHBoxLayout;
    topic_layout->addWidget( new QLabel( "vel_topic:" ));
    topic_list = new QComboBox(this);
    topic_list->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    auto spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    driveWidget_ = new DriveWidget();
    topic_layout->addWidget(topic_list);
    topic_layout->addItem(spacer);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(topic_layout);
    layout->addWidget(driveWidget_);

    setLayout(layout);

    QTimer* out_timer = new QTimer(this);

    connect(driveWidget_, &DriveWidget::outputVelocity, this, &PurePursuit::setVel);
    connect(out_timer, &QTimer::timeout, this, &PurePursuit::loop);
    connect(topic_list, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this, &PurePursuit::setTopic);

    out_timer->start(100);
    driveWidget_->setEnabled(false);
}

void PurePursuit::loop() {
    changeComboex(topic_list);
    sendVel();
}

void PurePursuit::sendVel() {
    if( ros::ok() && vel_pub){
        if(control_type == ACKERMANN){
            ackermann_msgs::AckermannDrive msg;
            msg.acceleration = 1;
            msg.jerk = 1;
            msg.speed = 1;
            msg.steering_angle = 1;
            msg.steering_angle_velocity = 2;
            vel_pub.publish(msg);
            // qDebug()
        } else if(control_type == TWIST){
            geometry_msgs::Twist msg;
            msg.linear.x = -1;
            msg.linear.y = 0;
            msg.linear.z = 0;
            msg.angular.x = 0;
            msg.angular.y = 0;
            msg.angular.z = 1;
            vel_pub.publish( msg );
        }
        if(control_type == ACKERMANN_Stamped){
            ackermann_msgs::AckermannDriveStamped msg;
            msg.header.stamp = ros::Time::now();
            msg.drive.acceleration = 1;
            msg.drive.jerk = 1;
            msg.drive.speed = 1;
            msg.drive.steering_angle = 1;
            msg.drive.steering_angle_velocity = 2;
            vel_pub.publish(msg);
            // qDebug()
        } else if(control_type == TWIST_Stamped){
            geometry_msgs::TwistStamped msg;
            msg.header.stamp = ros::Time::now();
            msg.twist.linear.x = -1;
            msg.twist.linear.y = 0;
            msg.twist.linear.z = 0;
            msg.twist.angular.x = 0;
            msg.twist.angular.y = 0;
            msg.twist.angular.z = 1;
            vel_pub.publish( msg );
        }
    }
}

void PurePursuit::updateTopic() {
    setTopic(output_topic_editor_->text());
}

void PurePursuit::setVel(float linear_velocity_, float angular_velocity) {
    this->linear_vel = linear_velocity_;
    this->angular_vel = angular_velocity;
}

void PurePursuit::setTopic(const QString &topic) {
    if(topic != output_topic_){
        output_topic_ = topic;
        if(output_topic_ == ""){
            vel_pub.shutdown();
        } else {
            std::string str = vel_topic_list[topic.toStdString()].c_str();
            if(  str == "ackermann_msgs/AckermannDrive"){
                if(control_type != ACKERMANN)
                    vel_pub.shutdown();
                control_type = ACKERMANN;
                vel_pub = nh_.advertise<ackermann_msgs::AckermannDrive>(output_topic_.toStdString(), 1);
                qDebug() << "change topic type to ackermann";
            }
            if(  str == "ackermann_msgs/AckermannDriveStamped"){
                if(control_type != ACKERMANN_Stamped)
                    vel_pub.shutdown();
                control_type = ACKERMANN_Stamped;
                vel_pub = nh_.advertise<ackermann_msgs::AckermannDriveStamped>(output_topic_.toStdString(), 1);
                qDebug() << "change topic type to ackermann stamped";
            }
            if(  str == "geometry_msgs/Twist"){
                if(control_type != TWIST)
                    vel_pub.shutdown();
                control_type = TWIST;
                vel_pub = nh_.advertise<geometry_msgs::Twist>(output_topic_.toStdString(), 1);
                qDebug() << "change topic type to twist";
            }
            if(  str == "geometry_msgs/TwistStamped"){
                if(control_type != TWIST_Stamped)
                    vel_pub.shutdown();
                control_type = TWIST_Stamped;
                vel_pub = nh_.advertise<geometry_msgs::TwistStamped>(output_topic_.toStdString(), 1);
                qDebug() << "change topic type to twist stamped";
            }
        }
    }
    Q_EMIT configChanged();     // rviz 的信号
    driveWidget_->setEnabled( output_topic_ != "" );
}

void PurePursuit::save(rviz::Config config) const {
    rviz::Panel::save( config );
    config.mapSetValue( "Topic", output_topic_ );
}

void PurePursuit::load( const rviz::Config& config )
{
    rviz::Panel::load( config );
    QString topic;
    if( config.mapGetString( "Topic", &topic ))
    {
        output_topic_editor_->setText( topic );
        updateTopic();
    }
}

/*
 * Retrieve list topic names and their types.
 * */
bool PurePursuit::getTopicTypes(ros::master::V_TopicInfo& topics) {
    XmlRpc::XmlRpcValue args, result, payload;
    args[0] = ros::this_node::getName();

    if (!ros::master::execute("getTopicTypes", args, result, payload, true)) {
        std::cout << "Failed!" << std::endl;
        return false;
    }
    topics.clear();
    std::cout << "----------TopicTypes----------" << std::endl;
    std::cout << "topic_name\t message_name" << std::endl;
    for (int i = 0; i < payload.size(); ++i) {
        topics.push_back(
                ros::master::TopicInfo(std::string(payload[i][0]),
                                       std::string(payload[i][1])));
        std::string v1 = std::string(payload[i][0]);
        std::string v2 = std::string(payload[i][1]);
        std::cout << v1.c_str() << "\t" << v2.c_str() << std::endl;
    }
    return true;
}

QList<QString> PurePursuit::getTopics(){
    ros::master::V_TopicInfo topic_info;  // 获取所有的ros话题
    // ros::master::getTopics(topic_info);
    QList<QString> topics;
    if(!getTopicTypes(topic_info)) return topics;
    vel_topic_list.clear();
    std::string ack_msg = "ackermann_msgs/AckermannDrive";
    std::string diff_msg = "geometry_msgs/Twist";
    
    for (ros::master::V_TopicInfo::const_iterator it = topic_info.begin(); it != topic_info.end(); it++)
    {
        if(it->datatype.find(ack_msg)!=it->datatype.npos
            || it->datatype.find(diff_msg)!=it->datatype.npos ){
            vel_topic_list.insert({it->name, it->datatype});
            topics.append(it->name.c_str());
            qDebug()<< "add vel topic " << it->name.c_str();
        }
    }
    return topics;
}

void PurePursuit::changeComboex(QComboBox *combox){
    QString selected= combox->currentText();
    auto topics = getTopics(); // ackermann
    if(!last_topics.empty())
        if(topics==last_topics){
//            qDebug() << "内容相同";
            return;
        }
    last_topics = topics;
    combox->clear();
    for (QList<QString>::const_iterator it = topics.begin(); it != topics.end(); it++)
    {
        QString label(*it);
        if(topics.size()>1 && label==""){
            continue;
        }
        if(selected==""){
            selected = label;
        }
        label.replace(" ", "/");
        combox->addItem(label, QVariant(*it));
    }
    // restore previous selection
    int index = combox->findText(selected);
    if (index == -1)
    {
        // add topic name to list if not yet in
        QString label(selected);
        label.replace(" ", "/");
        combox->addItem(label, QVariant(selected));
        index = combox->findText(selected);
    }
    combox->setCurrentIndex(index);
}


}   // end of namespace

// Tell pluginlib about this class.  Every class which should be
// loadable by pluginlib::ClassLoader must have these two lines
// compiled in its .cpp file, outside of any namespace scope.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(pure_rviz_plugin::PurePursuit,rviz::Panel )
// END_TUTORIAL
