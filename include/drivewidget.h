#ifndef DRIVEWIDGET_H
#define DRIVEWIDGET_H

#include <QWidget>

namespace Ui {
class DriveWidget;
}

class DriveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DriveWidget(QWidget *parent = nullptr);
    ~DriveWidget();

Q_SIGNALS:
    void outputVelocity( float linear, float angular );

private:
    Ui::DriveWidget *ui;
};

#endif // DRIVEWIDGET_H
