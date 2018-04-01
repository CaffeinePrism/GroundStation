#ifndef MISSION_H
#define MISSION_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQuickWidget>
#include <QQuickItem>
#include "mavlink.h"
#include "link.h"
#include <QObject>
#include "waypoint.h"
#include "obstacles.h"

#include "point.hpp"


struct FlyZone {
    qreal max_alt;
    qreal min_alt;
    QList<QVector2D> * boundary_points;
};

struct MissionWaypoints {
    QList<int> * actions;
    QList<QVector3D> * waypoints;
};

struct MissionTakeoff {
    float altitude;
    float pitch;
    float yawAngle;
};

struct MissionLanding {
    QList<Waypoint::WP> landingPath;
    QVector2D landingPoint;
    float abortAlt;
    float precisionLandMode; // 0=normal, 1=opportunistic, 2=required
};

class Mission : public QObject {
    Q_OBJECT
public:
    explicit Mission(QObject *parent = nullptr);

    Mission(QJsonObject);
    Mission(QJsonObject mission_obj, QJsonDocument obstacles_doc);
    Mission(const Mission& mission);

    int id;
    bool active;
    QVector2D home_pos;
    QVector2D air_drop_pos;
    QVector2D off_axis_odlc_pos;
    QVector2D emergent_last_known_pos;
    MissionWaypoints mission_waypoints;
    MissionTakeoff mission_takeoff;
    MissionLanding mission_landing;
    QList<QVector3D> * search_grid_points;
    QList<FlyZone> * fly_zones;
    Obstacles obstacles;

    QList<QPolygonF> get_obstacles();
    void loadWaypoint(mavlink_mission_request_t mrequest);
    void printJDoc();
    Point toECEF(double lat, double lon, double alt);
    QVector<Waypoint::WP> constructWaypoints();
    uint16_t waypointLength();
    void setActions_wp();
    Obstacles getObstacles();
    QList<QVector3D> get3DPath();
    QList<int> getActions();

signals:
    void loadToUAV(int seq, int cmd, float params[]);

private:
    QVector2D posToPoint(QJsonObject obj);
    QVector3D posTo3DPoint(QJsonObject obj);

    MissionWaypoints setMissionWaypoints(QJsonArray pointArray);
    QList<FlyZone> * setFlyZones(QJsonArray flyZoneArray);
    QList<QVector2D> * setPoints(QJsonArray pointArray);
    QList<QVector3D> * set3DPoints(QJsonArray pointArray);
    double meters_to_deg(double meters, double latitude)
    {
        return (meters / (111.32 * 1000 * cos(latitude * (M_PI / 180))));
    }

    void defaultTakeoff();
    void defaultLand();
    Waypoint::WP generateHomePositionWP();
    Waypoint::WP generateTakeoffWP();
    QVector<Waypoint::WP> generateLandingWP(int lastid);
};

#endif // MISSION_H
