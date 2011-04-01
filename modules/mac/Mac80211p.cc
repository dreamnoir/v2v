/*
 * Mac80211p.cpp
 *
 *  Created on: 2011-03-23
 *      Author: Kyle
 */

#include "Mac80211p.h"
#include "Decider80211.h"

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
	if (msg->getKind() == Decider80211::BITERROR)
	{
		notRecVec.record(1);
		ev << "bit error at 802.11p mac layer sending up to network" << endl;
		sendControlUp(msg->dup());
	}
	Mac80211::handleLowerControl(msg);
}
