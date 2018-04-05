#ifndef MISSIONWIDGET_H
#define MISSIONWIDGET_H

#include <QWidget>
#include "interop.h"
#include "mission.h"
#include "interop.h"
#include "obstacles.h"
#include "mapwidget.h"
#include "waypoint.h"
#include <QLineEdit>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QGeoCoordinate>
#include <QTableView>
#include "point.hpp"
#include "plan_mission.hpp"
#include <QFile>
#include "style.h"

namespace Ui {
class MissionWidget;
}

class MissionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MissionWidget(QWidget *parent = 0);

    void writeButtonClicked();
    void readButtonClicked();
    void clearButtonClicked();
    void setCurrentButtonClicked();
    bool hasMission();
    void generateMission();
    void getMissions(Interop * i);
    QStandardItemModel * createMissionModel(Mission * mission);
    void setTableModel(QTableView * tableView, QStandardItemModel * model);

    QVector<Mission*> * missions;
    Mission * generatedMission = nullptr;
    Mission * interopMission = nullptr;
    QStandardItemModel * model;
    Obstacles obstacles;
    MapWidget * mapWidget;

    Style style;

    bool test_mission = true;

    ~MissionWidget();

signals:
    void drawMission(Mission * selectedMission);
    void drawWaypoints(QList<QVector2D> * wps);
    void drawObstacle(QPolygonF poly, QColor color, QString label);
    void clearMissions();
    void setCurrentMision(uint16_t i);
    void readMissionsSignal();
    void writeMissionsSignal(QVector<Waypoint::WP> waypoints, uint16_t size);
    void clearMap();

    void selectWaypoint(int wpNum);
    void editMode(bool editing);
    void moveWaypointSignal(int wpNum, QVector3D newCoord);

public slots:
    void readMissions(Waypoint::WP * waypoints, uint16_t size);
    void writeMissionsStatus(bool success);
    void updateInteropMission(int index);
    void updateDraw(int index);
    void moveWaypoint(int wpNum, int key);

private:
    void testOutputJSON(QJsonObject o, int i);
    QJsonObject testReadJSON_mission(QString n);
    QJsonDocument testReadJSON_obstacle();

    Ui::MissionWidget *ui;
};

#endif // MISSIONWIDGET_H
