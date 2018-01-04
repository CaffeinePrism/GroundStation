#ifndef UAVBUTTON_H
#define UAVBUTTON_H

#include <QWidget>
#include <QPropertyAnimation>
#include "mavlink.h"
#include <QGraphicsColorizeEffect>
#include "QTime"
#include "QTimer"
namespace Ui {
class UAVButton;
}

class UAVButton : public QWidget
{
    Q_OBJECT

public:
    explicit UAVButton(QWidget *parent = 0);
    ~UAVButton();

    void updateBattery(mavlink_battery_status_t battery);
    void updateHeartbeat(mavlink_heartbeat_t heartbeat);
    bool downbeat = false;
    QPropertyAnimation * pulse;
    QGraphicsColorizeEffect * eEffect;
    QColor changeColor;
    QTimer * timer;
    void timeout();
    void doit();
private:
    Ui::UAVButton *ui;
};

#endif // UAVBUTTON_H