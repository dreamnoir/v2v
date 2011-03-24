/*
 * WSMNetwLayer.cpp
 *
 *  Created on: 2011-03-15
 *      Author: Kyle
 */

#include "WSMNetwLayer.h"
#include "BaseMacLayer.h"
#include "WSMPkt_m.h"
#include "NetwControlInfo.h"
#include "NetwToMacControlInfo.h"

Define_Module(WSMNetwLayer);

void WSMNetwLayer::initialize(int stage)
{
	BaseNetwLayer::initialize(stage);
}

NetwPkt* WSMNetwLayer::encapsMsg(cPacket *appPkt) {
    int macAddr;
    int netwAddr;

    EV <<"in encaps...\n";

    WSMPkt *pkt = new WSMPkt(appPkt->getName(), appPkt->getKind());
    pkt->setBitLength(headerLength);

    NetwControlInfo* cInfo = dynamic_cast<NetwControlInfo*>(appPkt->removeControlInfo());

    if(cInfo == 0){
	EV << "warning: Application layer did not specifiy a destination L3 address\n"
	   << "\tusing broadcast address instead\n";
	netwAddr = L3BROADCAST;
    } else {
	EV <<"CInfo removed, netw addr="<< cInfo->getNetwAddr()<<endl;
        netwAddr = cInfo->getNetwAddr();
	delete cInfo;
    }

    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(netwAddr);
    EV << " netw "<< myNetwAddr << " sending packet" <<endl;
    if(netwAddr == L3BROADCAST) {
        EV << "sendDown: nHop=L3BROADCAST -> message has to be broadcasted"
           << " -> set destMac=L2BROADCAST\n";
        macAddr = L2BROADCAST;
    }
    else{
        EV <<"sendDown: get the MAC address\n";
        macAddr = arp->getMacAddr(netwAddr);
    }

    pkt->setControlInfo(new NetwToMacControlInfo(macAddr));

    //encapsulate the application packet
    pkt->encapsulate(appPkt);
    EV <<" pkt encapsulated\n";
    return pkt;
}

void WSMNetwLayer::handleLowerControl(cMessage* msg)
{
	switch (msg->getKind())
	{
		case BaseMacLayer::TX_OVER:
			delete msg;
			break;
		default:
			EV << "WSMNetwLayer does not handle control messages called " << msg->getName() << endl;
			delete msg;
	}
}
