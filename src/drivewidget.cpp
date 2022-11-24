#include "drivewidget.h"
#include "ui_drivewidget.h"

DriveWidget::DriveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveWidget)
{
    ui->setupUi(this);
}

DriveWidget::~DriveWidget()
{
    delete ui;
}
