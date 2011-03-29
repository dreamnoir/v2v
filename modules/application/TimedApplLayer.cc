/*
 * TimedApplLayer.cpp
 *
 *  Created on: 2011-02-24
 *      Author: Kyle
 */

#include "TimedApplLayer.h"
#include "Move.h"
#include "CCWSApplPkt_m.h"
#include "NetwControlInfo.h"
#include "Coord.h"
#include <SimpleAddress.h>

Define_Module(TimedApplLayer);

void TimedApplLayer::Statistics::initialize()
{
	sentUpdates = 0;
	receivedUpdates = 0;
}

void TimedApplLayer::Statistics::recordScalars(cSimpleModule& module)
{
	module.recordScalar("Sent Updates", sentUpdates);
	module.recordScalar("Received Updates", receivedUpdates);
}

void TimedApplLayer::finish()
{
	stats.recordScalars(*this);
}

void TimedApplLayer::initialize(int stage)
{

    BaseApplLayer::initialize(stage);

	if(stage == 0)
	{
		stats.initialize();
		delayTimer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER );
		errorVec.setName("spe error");
		delay = par("delay").doubleValue();
		Move moveBBItem;
		catMove = utility->subscribe(this, &moveBBItem, findHost()->getId());
	}
	else if(stage==1)
	{
		scheduleAt(simTime() + delay + dblrand(), delayTimer);
	}
}

void TimedApplLayer::handleLowerMsg(cMessage* msg)
{
    CCWSApplPkt *m;
    switch( msg->getKind() ){
    case BROADCAST_MESSAGE:
        m = static_cast<CCWSApplPkt *>(msg);
        EV << "Received a broadcast packet from host["<<m->getSrcAddr()<<"] -> sending reply\n";
        ev << "speed=" << m->getSpeed() << " | accel=" << m->getAccel() << "| angle=" << m->getAngle() << " | x=" << m->getX() << " | y=" << m->getY() << endl;

        stats.receivedUpdates++;
        break;
	break;
    default:
	EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
        delete msg;
    }
}

void TimedApplLayer::handleSelfMsg(cMessage *msg)
{
	sendBroadcast();
	delayTimer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER );
	scheduleAt(simTime() + delay, delayTimer);
}

void TimedApplLayer::sendBroadcast()
{
	CCWSApplPkt *pkt = new CCWSApplPkt("BROADCAST_MESSAGE", BROADCAST_MESSAGE);
    pkt->setDestAddr(-1);
    // we use the host modules getIndex() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setBitLength(headerLength);

    pkt->setAccel(spe.getAcceleration());
    //pkt->setX(this->x);
    //pkt->setY(this->y);
    pkt->setSpeed(spe.getSpeed());
    //pkt->setAngle(this->angle);

    // set the control info to tell the network layer the layer 3
    // address;
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );

    EV << "Sending broadcast location packet!   " << simTime() << endl ;

    stats.sentUpdates++;

    sendDown( pkt );
}

void TimedApplLayer::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
	BaseModule::receiveBBItem(category, details, scopeModuleId);
	if(category == catMove)
	{
		const Move* m = static_cast<const Move*>(details);
		Coord estimate = spe.getCurrentPosition(simTime());
		double diff = estimate.distance(m->getStartPos());
		ev << "difference in position is " << diff << endl;

		errorVec.record(diff);

		spe.updatePosition(m->getStartPos(), m->getSpeed(), m->getDirection());
	}
}

TimedApplLayer::~TimedApplLayer()
{
	cancelAndDelete(delayTimer);
}
