/********************************************************************************
** Form generated from reading UI file 'drivewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRIVEWIDGET_H
#define UI_DRIVEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "plotpanel.h"

QT_BEGIN_NAMESPACE

class Ui_DriveWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    PlotRoad *widget;

    void setupUi(QWidget *DriveWidget)
    {
        if (DriveWidget->objectName().isEmpty())
            DriveWidget->setObjectName(QString::fromUtf8("DriveWidget"));
        DriveWidget->resize(900, 811);
        verticalLayout_2 = new QVBoxLayout(DriveWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout_2->addLayout(horizontalLayout);

        widget = new PlotRoad(DriveWidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout_2->addWidget(widget);

        verticalLayout_2->setStretch(1, 1);

        retranslateUi(DriveWidget);

        QMetaObject::connectSlotsByName(DriveWidget);
    } // setupUi

    void retranslateUi(QWidget *DriveWidget)
    {
        DriveWidget->setWindowTitle(QApplication::translate("DriveWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DriveWidget: public Ui_DriveWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRIVEWIDGET_H
