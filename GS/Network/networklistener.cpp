#include "networklistener.h"
#include "net.h"
#include <QtCore>
#include <QDebug>
#include <iostream>
#include <QString>
#include "gsserver.h"

#include "ackpacket.h"
#include "actionpacket.h"
#include "telemetrypacket.h"
#include "infopacket.h"
#include "packet.h"

#define BUFSIZE 4096

using namespace std;

NetworkListener::NetworkListener(messagebox *myMessagebox, GsServer* server){
    this->myMessageBox = myMessagebox;
    this->server = server;
    std::cout << "New NetworkListener created." << std::endl;
    bind(NET::LISTEN_PORT);
    listening = false;

    //connect(&startThread,SIGNAL(started()),this,SLOT(start()));
    //connect(this,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));

    connect(this,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
}

NetworkListener::NetworkListener(messagebox *myMessagebox, int UAVid, GsServer* server):NetworkListener(myMessagebox, server) {
    this->UAVid = UAVid;
    listening = false;
}

NetworkListener::~NetworkListener() {
    stop();
}

void NetworkListener::processPendingDatagrams(){
    if(!listening){
        return;
    }
    static int pack_number = 1;
    QByteArray datagram;
    datagram.resize(pendingDatagramSize());
    readDatagram(datagram.data(), datagram.size());

    Protocol::Packet* incPack = Protocol::Packet::Parse((uint8_t*)datagram.data(), datagram.size());
    Protocol::PacketType type = incPack->get_type();
    if (type == Protocol::PacketType::Ack){
        std::cout<< "AckPacket Recieved" << std::endl;
        Protocol::AckPacket *ackPacket = (Protocol::AckPacket*)incPack;
        myMessageBox->addAckPacket(*ackPacket);
        server->recieveAckPacket(ackPacket);
    } else if (type == Protocol::PacketType::Action){
        std::cout<< "ActionPacket Recieved" << std::endl;
        Protocol::ActionPacket *actionPacket = (Protocol::ActionPacket*)incPack;
        Protocol::Waypoint test_wp;
        test_wp = actionPacket->GetWaypoint();
        std::cout << pack_number << " Latitude: " << test_wp.lat << " Longitude: " << test_wp.lon <<  " Alt: " <<  test_wp.alt  << std::endl;
        ++pack_number;

        ///\todo remove this so that the red line is dependent on telem packets rather than action packets
        myMessageBox->addActionPacket(*actionPacket);
    } else if (type == Protocol::PacketType::Telem){
        std::cout<< "TelemPacket Recieved" << std::endl;
        Protocol::TelemetryPacket *telemPacket = (Protocol::TelemetryPacket*)incPack;
        //std::cout << pack_number << " Latitude: " << telemPacket << " Longitude: " << telemPacket->lon << std::endl;
        ++pack_number;
        myMessageBox->addTelemetryPacket(*telemPacket);
        server->addPacketToMission(*telemPacket);
        //emit this->packetRecieved(telemPacket);

        TelemetryData td(telemPacket);
        emit telemDataRecieved(td);
    } else if (type == Protocol::PacketType::Info){
        std::cout<< "InfoPacket Recieved" << std::endl;
        Protocol::InfoPacket *infoPacket = (Protocol::InfoPacket*)incPack;
        myMessageBox->addInfoPacket(*infoPacket);
    } else {
        std::cout<< "UNKNOWN PACKET TYPE RECIEVED!" << std::endl;
    }
    return;
}
void NetworkListener::startListening(){
    start();
}

void NetworkListener::start(){
    listening = true;
}

void NetworkListener::stop(){
    //if(isOpen()){
        abort();
    //}
    disconnect(this,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    listening = false;
    std::cout << "Stopping NetworkListener...";
}
