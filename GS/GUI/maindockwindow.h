#ifndef MAINDOCKWINDOW_H
#define MAINDOCKWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QStackedWidget>
#include <QQuickWidget>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <thread>
#include <QDebug>
#include <QQuickItem>

#include "graphwidget.h"
#include "tablewidget.h"
#include "toolbar.h"
#include "timerwidget.h"
#include "interop.h"
#include "mission.h"
#include "obstacles.h"
#include "tcplink.h"
#include "udplink.h"
#include "timer.h"
#include "decoder.h"
#include "movementwidget.h"
#include "statuswidget.h"
#include "missiondetailswindow.h"
#include "tcplink.h"
#include "actionwidget.h"
#include "qfiwidget.h"

namespace Ui {
class MainDockWindow;
}

class MainDockWindow : public QMainWindow
{
    Q_OBJECT

public:
    QStackedWidget * centralWidget;
    QToolBar * toolBar;
    UdpLink * link;
    TcpLink * tlink;
    explicit MainDockWindow(QWidget *parent = 0);
    ~MainDockWindow();

    QQuickWidget * createQmlWidget(QUrl qmlSource, QWidget * parent = 0);
    QDockWidget * createDockWidget(const QString &title, Qt::DockWidgetArea area, QWidget * child, QWidget * parent);
    void testFind();
    void testMav();
    void sendCommand();

private slots:
    void hideDockWidgets();
    void closeDockWidgets();
    void updateMovingObjects(QQuickWidget * mapWidget);

private:
    Ui::MainDockWindow *ui;
    Interop * interop;
};

#endif // MAINDOCKWINDOW_H
