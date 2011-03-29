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
		nerrorVec.setName("nve error");
		delay = par("delay").doubleValue();
		maxVehicles = (int) par("maxVehicles").doubleValue();
		Move moveBBItem;
		catMove = utility->subscribe(this, &moveBBItem, findHost()->getId());
	}
	else if(stage==1)
	{
		nve = new PositionEstimator*[maxVehicles];
		for (int i=0; i<maxVehicles; i++)
			nve[i] = 0;
		scheduleAt(simTime() + delay + dblrand(), delayTimer);
	}
}

void TimedApplLayer::handleLowerMsg(cMessage* msg)
{
	int from;
	Coord estimate, now;
	double diff;
    CCWSApplPkt *m;
    switch( msg->getKind() ){
    case BROADCAST_MESSAGE:
        m = static_cast<CCWSApplPkt *>(msg);
        if (debug) EV << "Received a broadcast packet from host["<<m->getSrcAddr()<<"] -> sending reply\n";
        if (debug) ev << "speed=" << m->getSpeed() << " | accel=" << m->getAccel() << "| angleX=" << m->getAngleX() << "| angleY=" << m->getAngleY() << " | x=" << m->getX() << " | y=" << m->getY() << endl;

        stats.receivedUpdates++;

        from = m->getSrcAddr();

        if (nve[from] == 0)
        	nve[from] = new PositionEstimator;

        if (m->getCreationTime() - nve[from]->getLastUpdated() < 5)
        {
			estimate = nve[from]->getCurrentPosition(simTime());
			now.setX(m->getX());
			now.setY(m->getY());
			diff = estimate.distance(now);
			if (debug) ev << "difference in position is " << diff << endl;

			if (diff > 0)
				nerrorVec.record(diff);
        }
        nve[from]->updatePosition(m->getX(), m->getY(), m->getSpeed(), m->getAngleX(), m->getAngleY(), m->getCreationTime());

        break;
    default:
    	if (debug) EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
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
    pkt->setX(spe.getCurrentPosition(simTime()).getX());
    pkt->setY(spe.getCurrentPosition(simTime()).getY());
    pkt->setSpeed(spe.getSpeed());
    pkt->setAngleX(spe.getAngle().getX());
    pkt->setAngleY(spe.getAngle().getY());

    // set the control info to tell the network layer the layer 3
    // address;
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );

    if (debug) EV << "Sending broadcast location packet!   " << simTime() << endl ;

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
		if (debug) ev << "difference in position is " << diff << endl;

		if (diff > 0)
			errorVec.record(diff);

		spe.updatePosition(m->getStartPos(), m->getSpeed(), m->getDirection());
	}
}

TimedApplLayer::~TimedApplLayer()
{
	cancelAndDelete(delayTimer);
}
