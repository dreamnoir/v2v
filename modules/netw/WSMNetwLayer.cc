/*
 * WSMNetwLayer.cpp
 *
 *  Created on: 2011-03-15
 *      Author: Kyle
 */

#include "WSMNetwLayer.h"
#include "BaseMacLayer.h"
#include "WAVEControlInfo.h"
#include "NetwTo80211pControlInfo.h"

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

    WAVEControlInfo* control = (WAVEControlInfo*) appPkt->removeControlInfo();

    netwAddr = control->getNetwAddr();

	if (netwAddr != L3BROADCAST)
	{
		error("Can't lookup network address!");
	}
	else
	{
		macAddr = L2BROADCAST;
	}

	pkt->setBitLength(this->headerLength);
    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(netwAddr);
    pkt->setChNum(control->getChannel());
    pkt->setDataRate(control->getDataRate());
    pkt->setTxPower(control->getTxPower());

    EV << " netw "<< myNetwAddr << " sending packet" <<endl;

    pkt->setControlInfo(new NetwTo80211pControlInfo(macAddr, NetwTo80211pControlInfo::A2));

    //encapsulate the application packet
    pkt->encapsulate(appPkt);
    EV <<" pkt encapsulated\n";
    return pkt;
}

cMessage* WSMNetwLayer::decapsMsg(NetwPkt *msg)
{
    cMessage *m = msg->decapsulate();
    WSMPkt* wmsg = (WSMPkt*) msg;
    m->setControlInfo(new WAVEControlInfo(wmsg->getSrcAddr(), wmsg->getChNum(), wmsg->getDataRate(), wmsg->getTxPower()));
    // delete the netw packet
    delete msg;
    return m;
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
