#include "CCWSApplLayer.h"

#include "WAVEAddress.h"
#include "WAVEControlInfo.h"

#include "Move.h"
#include "CCWSApplPkt_m.h"
#include "Coord.h"

Define_Module(CCWSApplLayer);

void CCWSApplLayer::Statistics::initialize()
{
	sentUpdates = 0;
	receivedUpdates = 0;
	shortDelay = 0;

	// setup vector staticis
	speErrorVec.setName("SPE Error");
	nveErrorVec.setName("NVE Error");
	nveDistanceVec.setName("NVE Received Distance");
	nveLatencyVec.setName("NVE Latency");
	visibleVec.setName("Visible");
	mvisibleVec.setName("Potentially Visible");
	thresholdVec.setName("Threshold Error");
	nveVec.setName("NVE Tracked");
	ndeletecVec.setName("NVE Deleted");

}

void CCWSApplLayer::Statistics::recordScalars(cSimpleModule& module)
{
	module.recordScalar("Sent Updates", sentUpdates);
	module.recordScalar("Received Updates", receivedUpdates);
	module.recordScalar("Short Delay Intervals", shortDelay);
}

void CCWSApplLayer::finish()
{
	stats.recordScalars(*this);
}

void CCWSApplLayer::initialize(int stage)
{

    BaseApplLayer::initialize(stage);

	if(stage == 0)
	{
		// get parameters
		delay = par("delay").doubleValue();
		maxVehicles = (int) par("maxVehicles").doubleValue();
		thresholdMode = par("thresholdMode").boolValue();
		thresholdSize = par("thresholdSize").doubleValue();
		nveTimeout = (int) par("nveTimeout").doubleValue();
		retransmit = par("retransmit").boolValue();
		retransmitTime = par("retransmitTime").doubleValue();
		autoRetransmitTime = par("autoRetransmitTime").doubleValue();
		txPower = (int) par("txPower").doubleValue();
		runUp = par("runUp").doubleValue();
		visionOn = par("vision").boolValue();

		// find vision manager
		vm = dynamic_cast<VisionManager*>(simulation.getModuleByPath("vision"));

		// not registered with vision manager yet
		isRegistered = false;

		// initialize statics module
		stats.initialize();

		// subscribe to movement updates
		Move moveBBItem;
		catMove = utility->subscribe(this, &moveBBItem, findHost()->getId());

	}
	else if(stage==1)
	{
		double add = 0;

		// create and initialise empty array of NVE
		nve = new PositionEstimator*[maxVehicles];
		for (int i=0; i<maxVehicles; i++)
			nve[i] = 0;

		if (runUp > SIMTIME_DBL(simTime()))
			add = runUp - SIMTIME_DBL(simTime());

		// start position update timer if in threshold mode otherwise start broadcast timer
		if (!thresholdMode)
		{
			timer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER);
			scheduleAt(simTime() + delay + dblrand() + add, timer);
		}
		else
		{
			timer = new cMessage( "update-timer", CHECK_POSITION_UPDATE);
			scheduleAt(simTime() + delay + dblrand() + add, timer);
		}

	}
}

void CCWSApplLayer::handleLowerMsg(cMessage* msg)
{
	int from;
	Coord estimate, now;
	double distance;
    CCWSApplPkt *m;
    WAVEControlInfo* control;

    switch( msg->getKind() )
    {
		case CCWS_MESSAGE:

			// get packet and control information
			m = static_cast<CCWSApplPkt *>(msg);
			control = (WAVEControlInfo*) m->removeControlInfo();

			if (debug) EV << "Received a broadcast packet from host["<<m->getSrcAddr()<<"] -> sending reply\n";
			if (debug) ev << "control ch= " << control->getChannel() << " and txPower= " << control->getTxPower() << endl;
			if (debug) ev << "speed=" << m->getSpeed() << " | accel=" << m->getAccel() << "| angleX=" << m->getAngleX() << "| angleY=" << m->getAngleY() << " | x=" << m->getX() << " | y=" << m->getY() << endl;

			// record packet as received
			stats.receivedUpdates++;

			distance = spe.getPosition().distance(Coord(m->getX(), m->getY()));
			stats.nveDistanceVec.record(distance);

			// if no NVE exists create one
			from = m->getSrcAddr();
			if (nve[from] == 0)
				nve[from] = new PositionEstimator();

			stats.nveLatencyVec.record(simTime() - m->getUtc());

			// update nve position
			nve[from]->updatePosition(m->getX(), m->getY(), m->getSpeed(), m->getAngleX(), m->getAngleY(), m->getCreationTime());

			break;

		default:
			if (debug) EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;

    }
    delete msg;
}

void CCWSApplLayer::handleSelfMsg(cMessage *msg)
{
	double errorSize, delayTime;
	Coord current;
	bool send = false;

	switch(msg->getKind())
	{
	    case SEND_BROADCAST_TIMER:
			sendLocationUpdate();
			timer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER );
			scheduleAt(simTime() + delay, timer);
			break;

	    case RETRANSMIT_POSITION_UPDATE:

	    	sendLocationUpdate();
	    	timer = new cMessage( "update-timer", CHECK_POSITION_UPDATE);
	    	scheduleAt(simTime() + delay, timer);

	    	break;

	    case CHECK_POSITION_UPDATE:

	    	// delay until next check
	    	delayTime = delay;

	    	// get error between SPE and RPE
	    	current = spe.getCurrentPosition(simTime());
	    	errorSize = rpe.positionError(current, simTime());

	    	// if error larger than threshold or update time longer than update time
	    	if (simTime() - spe.getLastUpdated() > autoRetransmitTime)
	    	{
	    		send = true;
	    	}
	    	else if (errorSize > thresholdSize)
			{
				send = true;
			}
			else if (errorSize > 0.5*thresholdSize)
			{
				delayTime /= 5;
				stats.shortDelay++;
			}
			else if (errorSize > 0.75*thresholdSize)
			{
				delayTime /= 10;
				stats.shortDelay++;
			}

			if (send)
			{
				if (rpe.getNumberUpdates() > 1)
					stats.thresholdVec.record(errorSize);

				sendLocationUpdate();

				if (retransmit)
				{
					timer = new cMessage( "resend-timer", RETRANSMIT_POSITION_UPDATE);
					scheduleAt(simTime() + dblrand()*retransmitTime, timer);
				}
				else
				{
					timer = new cMessage( "update-timer", CHECK_POSITION_UPDATE);
					scheduleAt(simTime() + delay, timer);
				}
			}
			else
			{
				timer = new cMessage( "update-timer", CHECK_POSITION_UPDATE);
				scheduleAt(simTime() + delayTime, timer);
			}

	    	break;

	    default:
	    	ev << "CCWSApplLayer unknown self message" << endl;
	}

	delete msg;
}

void CCWSApplLayer::sendLocationUpdate()
{
	if (simTime() >= runUp)
	{
		CCWSApplPkt *pkt = new CCWSApplPkt("CCWS_MESSAGE", CCWS_MESSAGE);
		pkt->setDestAddr(-1);

		// we use the host modules getIndex() as a appl address
		pkt->setSrcAddr(myApplAddr());
		pkt->setBitLength(headerLength);

		pkt->setAccel(spe.getAcceleration());
		pkt->setX(spe.getCurrentPosition(simTime()).getX());
		pkt->setY(spe.getCurrentPosition(simTime()).getY());
		pkt->setSpeed(spe.getSpeed());
		pkt->setAngleX(spe.getAngle().getX());
		pkt->setAngleY(spe.getAngle().getY());
		pkt->setBitLength(headerLength);
		pkt->setUtc(simTime());

		// set the control info to tell the network layer the layer 3
		// address;
		pkt->setControlInfo(new WAVEControlInfo(L2BROADCAST, CCH, SIXMBS, txPower) );

		if (debug) EV << "Sending broadcast location packet!   " << simTime() << endl ;

		stats.sentUpdates++;

		// update remote rpe
		rpe.updatePosition(spe.getCurrentPosition(simTime()), spe.getSpeed(), spe.getAngle());

		sendDown(pkt);
	}
}

void CCWSApplLayer::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
	BaseModule::receiveBBItem(category, details, scopeModuleId);
	if(category == catMove)
	{
		if (simTime() >= runUp)
		{
			// get movement information
			const Move* m = static_cast<const Move*>(details);

			if (m->getStartPos().getX() != 0 && m->getStartPos().getY() != 0)
			{
				double diff = spe.positionError(m->getStartPos(), simTime());
				if (spe.getNumberUpdates() > 1)
					stats.speErrorVec.record(diff);

				// update SPE
				spe.updatePosition(m->getStartPos(), m->getSpeed(), m->getDirection());

				if (visionOn)
				{
					// vision updates
					if (isRegistered)
					{
						vm->updateVehiclePos(myApplAddr(), &(m->getStartPos()), &(m->getDirection()));
					}
					else
					{
						vm->registerVehicle(this, &(m->getStartPos()), &(m->getDirection()));
						isRegistered = true;
					}
					stats.visibleVec.record(vm->visible(myApplAddr()));
					stats.mvisibleVec.record(vm->maybeVisible(myApplAddr()));
				}

				int count = 0;
				int deleted = 0;
				for (int i=0; i<maxVehicles; i++)
				{
					if (nve[i] != 0)
					{
						if (simTime() - nve[i]->getLastUpdated() < nveTimeout && vm->vehicleExists(i))
						{
							count++;

							// make sure this isn't the first update and record position error if not
							if (nve[i]->getNumberUpdates() > 1)
							{
								double diff = nve[i]->positionError(vm->getVehiclePos(i), simTime());
								stats.nveErrorVec.record(diff);
							}
						}
						else
						{
							PositionEstimator* temp = nve[i];
							nve[i] = (PositionEstimator*) 0;
							delete (temp);
							deleted++;
						}
					}
				}
				stats.ndeletecVec.record(deleted);
				stats.nveVec.record(count);

			}
		}
	}
}

Coord CCWSApplLayer::getCurrentPos()
{
	return spe.getCurrentPosition(simTime());
}

CCWSApplLayer::~CCWSApplLayer()
{
	// unregister from VisionManager
	if (isRegistered)
		vm->unregisterVehicle(this);

	// delete array of NVEs
	delete [] nve;

	// cancel any timers
	if (timer != 0)
		cancelAndDelete(timer);
}
