//
// Created by lrm on 22-11-21.
//

#ifndef PURE_RVIZ_PLUGIN_PUREPURSUIT_H
#define PURE_RVIZ_PLUGIN_PUREPURSUIT_H

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include <rviz/panel.h>
#endif
#include <QComboBox>

#include "drivewidget.h"
#include "plotpanel.h"


#define TWIST     0    // 差速
#define ACKERMANN 1      // 阿克曼
#define TWIST_Stamped     2    // 差速
#define ACKERMANN_Stamped 3      // 阿克曼

class QLineEdit;

namespace pure_rviz_plugin{

//class DriveWidget;

class PurePursuit: public rviz::Panel{
    Q_OBJECT
public:
    PurePursuit(QWidget* parent=0);
    virtual void load(const rviz::Config& config);
    virtual void save(rviz::Config config) const;
    QList<QString> getTopics();
    void changeComboex(QComboBox *combox);
    bool getTopicTypes(ros::master::V_TopicInfo& topics);

public Q_SLOTS:
    void setVel(float linear_velocity_, float angular_velocity);
    void setTopic(const QString& topic);

protected Q_SLOTS:
    void loop();    // 作为主循环
    void sendVel();
    void updateTopic();

protected:
    DriveWidget* driveWidget_;
    QLineEdit* output_topic_editor_;
    QComboBox* topic_list;
    QString output_topic_;
    QList<QString> last_topics;
    PlotRoad *plot;

    ros::Publisher vel_pub;
    ros::NodeHandle nh_;

    float linear_vel;
    float angular_vel;

    unsigned char control_type;
    std::map<std::string, std::string> vel_topic_list;   // 阿克曼类型消息
};

}




#endif //PURE_RVIZ_PLUGIN_PUREPURSUIT_H
