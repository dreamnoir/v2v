/*
 * TimedApplLayer.cpp
 *
 *  Created on: 2011-02-24
 *      Author: Kyle
 */

#include "TimedApplLayer.h"
#include "Move.h"

Define_Module(TimedApplLayer);

void TimedApplLayer::Statistics::initialize()
{
	sentUpdates = 0;
	receivedUpdates = 0;
}

void TimedApplLayer::initialize(int stage)
{


	if(stage == 0)
	{
		TestApplLayer::initialize(stage);

		tVec.setName("transmissions");
		delay = par("delay").doubleValue();
		Move moveBBItem;
		catMove = utility->subscribe(this, &moveBBItem, findHost()->getId());
	}
	else if(stage==1)
	{
		scheduleAt(simTime() + delay + dblrand(), delayTimer);
	}
}

void TimedApplLayer::handleSelfMsg(cMessage *msg)
{
	TestApplLayer::sendBroadcast();
	delayTimer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER );
	tVec.record(simTime());
	scheduleAt(simTime() + delay, delayTimer);
}

void TimedApplLayer::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
	BaseModule::receiveBBItem(category, details, scopeModuleId);
	if(category == catMove)
	{
		const Move* m = static_cast<const Move*>(details);

		double speed = m->getSpeed();

		ev << " application layer received move going " << speed << " speed." << endl;
	}
}
