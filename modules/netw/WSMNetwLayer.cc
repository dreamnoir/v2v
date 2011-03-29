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

    appPkt->removeControlInfo();

	netwAddr = L3BROADCAST;
	macAddr = L2BROADCAST;

    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(netwAddr);
    EV << " netw "<< myNetwAddr << " sending packet" <<endl;

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
