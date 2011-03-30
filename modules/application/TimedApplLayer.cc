#include <SimpleAddress.h>

#include "TimedApplLayer.h"
#include "Move.h"
#include "CCWSApplPkt_m.h"
#include "NetwControlInfo.h"
#include "Coord.h"

Define_Module(TimedApplLayer);

void TimedApplLayer::Statistics::initialize()
{
	sentUpdates = 0;
	receivedUpdates = 0;
	shortDelay = 0;
}

void TimedApplLayer::Statistics::recordScalars(cSimpleModule& module)
{
	module.recordScalar("Sent Updates", sentUpdates);
	module.recordScalar("Received Updates", receivedUpdates);
	module.recordScalar("Short Delay Intervals", shortDelay);
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
		// get parameters
		delay = par("delay").doubleValue();
		maxVehicles = (int) par("maxVehicles").doubleValue();
		thresholdMode = par("thresholdMode").boolValue();
		thresholdSize = par("thresholdSize").doubleValue();

		// find vision manager
		vm = dynamic_cast<VisionManager*>(simulation.getModuleByPath("vision"));

		// not registered with vision manager yet
		isRegistered = false;

		// initialize statics module
		stats.initialize();

		// setup vector staticis
		errorVec.setName("spe error");
		nerrorVec.setName("nve error");
		visibleVec.setName("visible");
		thresholdVec.setName("Threshold Error");

		// subscribe to movement updates
		Move moveBBItem;
		catMove = utility->subscribe(this, &moveBBItem, findHost()->getId());

	}
	else if(stage==1)
	{
		// create and initialise empty array of NVE
		nve = new PositionEstimator*[maxVehicles];
		for (int i=0; i<maxVehicles; i++)
			nve[i] = 0;

		if (!thresholdMode)
		{
			timer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER);
			scheduleAt(simTime() + delay + dblrand(), timer);
		}
		else
		{
			timer = new cMessage( "update-timer", CHECK_POSITION_UPDATE);
			scheduleAt(simTime() + delay + dblrand(), timer);
		}

	}
}

void TimedApplLayer::handleLowerMsg(cMessage* msg)
{
	int from;
	Coord estimate, now;
	double diff;
    CCWSApplPkt *m;

    switch( msg->getKind() )
    {
		case BROADCAST_MESSAGE:
			m = static_cast<CCWSApplPkt *>(msg);
			if (debug) EV << "Received a broadcast packet from host["<<m->getSrcAddr()<<"] -> sending reply\n";
			if (debug) ev << "speed=" << m->getSpeed() << " | accel=" << m->getAccel() << "| angleX=" << m->getAngleX() << "| angleY=" << m->getAngleY() << " | x=" << m->getX() << " | y=" << m->getY() << endl;

			stats.receivedUpdates++;

			from = m->getSrcAddr();

			if (nve[from] == 0)
				nve[from] = new PositionEstimator;

			if (m->getCreationTime() - nve[from]->getLastUpdated() < 2.0)
			{
				diff = nve[from]->positionError(Coord(m->getX(), m->getY()), m->getCreationTime());
				if (debug) ev << "difference in position is " << diff << endl;

				if (diff >= 0)
					nerrorVec.record(diff);
			}
			nve[from]->updatePosition(m->getX(), m->getY(), m->getSpeed(), m->getAngleX(), m->getAngleY(), m->getCreationTime());

			break;
		default:
			if (debug) EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;

    }
    delete msg;
}

void TimedApplLayer::handleSelfMsg(cMessage *msg)
{
	double errorSize, delayTime;
	Coord current;

	switch(msg->getKind())
	{
	    case SEND_BROADCAST_TIMER:
			sendLocationUpdate();
			timer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER );
			scheduleAt(simTime() + delay, timer);
			break;

	    case CHECK_POSITION_UPDATE:

	    	delayTime = delay;
	    	current = spe.getCurrentPosition(simTime());
	    	errorSize = rpe.positionError(current, simTime());

			if (errorSize > thresholdSize)
			{
				thresholdVec.record(errorSize);
				sendLocationUpdate();
			}
			else if (errorSize > 0.75*thresholdSize)
			{
				delayTime /= 10;
				stats.shortDelay++;
			}

			timer = new cMessage( "update-timer", CHECK_POSITION_UPDATE);
			scheduleAt(simTime() + delayTime, timer);

	    	break;

	    default:
	    	ev << "CCWSApplLayer unknown self message" << endl;
	}

	delete msg;
}

void TimedApplLayer::sendLocationUpdate()
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

    // update remote rpe
    rpe.updatePosition(spe.getCurrentPosition(simTime()), spe.getSpeed(), spe.getAngle());

    sendDown( pkt );
}

void TimedApplLayer::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
	BaseModule::receiveBBItem(category, details, scopeModuleId);
	if(category == catMove)
	{
		// get movement information
		const Move* m = static_cast<const Move*>(details);

		if (m->getStartPos().getX() != 0 && m->getStartPos().getY() != 0)
		{
			double diff = spe.positionError(m->getStartPos(), simTime());
			if (diff >= 0)
				errorVec.record(diff);

			// update SPE
			spe.updatePosition(m->getStartPos(), m->getSpeed(), m->getDirection());

			// vision updates
			if (isRegistered)
			{
				vm->updateNicPos(this->getId(), &(m->getStartPos()));
			}
			else
			{
				vm->registerNic(this, &(m->getStartPos()));
				isRegistered = true;
			}

			visibleVec.record(vm->inRange(this->getId()));
		}
	}
}

TimedApplLayer::~TimedApplLayer()
{
	// unregister from VisionManager
	if (isRegistered)
		vm->unregisterNic(this);

	// delete array of NVEs
	delete [] nve;

	// cancel any timers
	if (timer != 0)
		cancelAndDelete(timer);
}
