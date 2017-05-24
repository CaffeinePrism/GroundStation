#ifndef MAPEXECUTION_H
#define MAPEXECUTION_H

#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QWebEngineView>
#include <QPair>
#include <QList>

#include "digitalclock.h"
#include "gsserver.h"
#include "tablemodel.h"
#include "qcomboboxdelegate.h"
#include "maprecap.h"
#include "missionrecap.h"
#include "options.h"
#include "messagebox.h"
#include "mission.h"
#include "mapwidget.h"

#include "ui_mapexecution.h"
#include "messagebox.h"
#include "connectiondialog.h"

#include "flightpath.h"


namespace Ui {
class MapExecution;
}

class MapExecution : public QDialog {

    Q_OBJECT

    friend class MainMDIDisplay;

public slots:
    void addNewMap();
    void plotPosition(double lat, double lng);
    void sendCoordToJSMap(double lat, double lng, int mapID);
    void drawFlightPath(FlightPath *flightPath);
    void updateTable(double lat, double lng);
    void sendFlightPlan();
    void updateStatusIndicator();
    void setupMapPaths();
    void receivePacket(Protocol::Packet* packet);

public:
    MapExecution(FlightPath* flightPath, QWidget *parent = 0);
    explicit MapExecution(QWidget *parent = 0);
    ~MapExecution();

    GsServer myServer;

    messagebox MyMessageBox;

    //messagebox MessageBox;

    Mission getMyMission() const;

    MapRecap *getMapRecap();

    void setMyMission(const Mission &value);

private slots:
    void onFinishButtonClicked();
    void on_returnHomeButton_clicked();
    void onCancelButtonClicked();
    void on_stopButton_clicked();
    void onStartButtonClicked();
    void addClickListener();

private:
    Ui::MapExecution *ui;
    TableModel *model;
    QButtonGroup *buttonGroup;
    double prevLat, prevLng, prevAlt;
    QTime prevTime;

    FlightPath myFlightPath;
    Mission myMission;
    MapWidget* map;

    bool missionStarted;
    QTimer *conTime;

signals:
    void timeToStartMapRecap();
};

#endif // MAPEXECUTION_H
