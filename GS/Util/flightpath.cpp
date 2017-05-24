#include "flightpath.h"
#include <QVector>
#include <iostream>
#include "dbmanager.h"

FlightPath::FlightPath()  {

}

FlightPath::FlightPath(QString filename)  {

    /// @todo any database initialization and error checking if necesary
    DbManager db(filename);

    foreach (FlightPathData fpData, db.flightPathGet()) {
        unsigned char* dataArray= DbManager::toArray(fpData.data);
        Protocol::ActionPacket ap(dataArray, kDataSize);
        if(ap.GetAction() == Protocol::ActionType::AddWaypoint){
            this->addNavAction(ap.GetWaypoint(), fpData.delay);
        } else {
            this->addCmdAction(ap.GetAction(), fpData.delay);
        }
    }

//    for (int i = 0; i < length; i++){

//        /// @todo update these two lines with values from the database
//        double delay = i*10;
//        unsigned char data[40] = {};

//        Protocol::ActionPacket ap(data,40);
//        if(ap.GetAction() == Protocol::ActionType::AddWaypoint){
//            this->addNavAction(ap.GetWaypoint(),delay);
//        } else {
//            this->addCmdAction(ap.GetAction(), delay);
//        }
//    }

    /// @todo any remaining database functions for safe file handling if needed
    db.close();
}

FlightPath::~FlightPath() {

}

void FlightPath::addCmdAction(Protocol::ActionType action, double delay){
    this->append(new TimedAction(action,delay));
}

void FlightPath::addNavAction(Protocol::Waypoint wp, double delay){
    this->append(new TimedAction(wp,delay));
}

QList<Protocol::Waypoint>* FlightPath::getOrderedWaypoints(){
    QList<Protocol::Waypoint> *rtnList = new QList<Protocol::Waypoint>;
    QList<TimedAction*> waypointActions;

    for (TimedAction* action : *this) {
        if(action->hasWaypoint()){
            waypointActions.append(action);
        }
    }

    while (!waypointActions.empty()){
        double earliestTime = std::numeric_limits<double>::infinity();
        TimedAction* earliestAction;
        for(int i = 0; i < waypointActions.length(); i++){
            TimedAction* currentAction = waypointActions.at(i);
            if(currentAction->second < earliestTime){
                earliestTime = currentAction->second;
                earliestAction = currentAction;
            }
        }
        waypointActions.removeOne(earliestAction);
        rtnList->append(earliestAction->first->GetWaypoint());
    }

    return rtnList;
}

bool FlightPath::save(QString filename){
    DbManager db(filename);
    db.flightPathClear();

    for(int i = 0; i < this->length(); i++){
        FlightPathData fpData;
        fpData.actionNum = i;
        fpData.delay = this->at(i)->second;

        Protocol::ActionPacket* ap = this->at(i)->first;
        unsigned char data[40];
        ap->GetBytes(data,40);
        fpData.data = DbManager::toVector(data, 40);

        db.flightPathAdd(fpData);
    }

    db.flightPathSaveToFile();
    db.close();
    return true;
}

