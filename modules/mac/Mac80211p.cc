/*
 * Mac80211p.cpp
 *
 *  Created on: 2011-03-23
 *      Author: Kyle
 */

#include "Mac80211p.h"
#include "Decider80211.h"
#include "MacToPhyControlInfo.h"
#include "WSMPkt_m.h"

Define_Module(Mac80211p);

void Mac80211p::initialize(int stage)
{
	Mac80211::initialize(stage);
	if (stage == 0)
	{
		recVec.setName("received");
		notRecVec.setName("failed");
	}
}

void Mac80211p::handleLowerMsg(cMessage *msg)
{
	recVec.record(1);
	Mac80211::handleLowerMsg(msg);
}

void Mac80211p::handleLowerControl(cMessage* msg)
{
	if (msg->getKind() == Decider80211::BITERROR || msg->getKind() == Decider80211::COLLISION)
	{
		notRecVec.record(1);
		ev << "bit error at 802.11p mac layer sending up to network" << endl;
		sendControlUp(msg->dup());
	}
	Mac80211::handleLowerControl(msg);
}

/**
 *  Send a BROADCAST frame.Called by handleContentionTimer()
 */
void Mac80211p::sendBROADCASTframe()
{
	double txPowerPkt;

    // send a copy of the frame in front of the queue
    Mac80211Pkt *frame = static_cast<Mac80211Pkt *>(fromUpperLayer.front()->dup());

    //get inner packet for txPower
    WSMPkt* inner = (WSMPkt*) frame->getEncapsulatedPacket();
    txPowerPkt = inner->getTxPower();
    if (txPowerPkt == 0) txPowerPkt = txPower;

    double br = inner->getDataRate();

    simtime_t duration = packetDuration(frame->getBitLength(), br);
    Signal* signal = createSignal(simTime(), duration, txPowerPkt, br);

    MacToPhyControlInfo *pco = new MacToPhyControlInfo(signal);

    frame->setControlInfo(pco);
    frame->setKind(BROADCAST);

    sendDown(frame);
    // update state and display
    setState(BUSY);
}
