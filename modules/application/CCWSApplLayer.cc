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
	thresholdViolations = 0;
	timeViolations = 0;

	// setup vector staticis
	speErrorVec.setName("spe-error");
	rpeTransmitVec.setName("rpe-interval");
	nveErrorVec.setName("nve-error");
	nveDistanceVec.setName("nve-distance");
	nveLatencyVec.setName("nve-latency");
	visibleVec.setName("visible");
	mvisibleVec.setName("in-range");
	thresholdVec.setName("threshold-error");
	nveVec.setName("nve-tracked");
	ndeletecVec.setName("nve-deleted");

	nveErrorVec1.setName("nve-error1");
	nveVec1.setName("nve-tracked1");
	nveErrorVec2.setName("nve-error2");
	nveVec2.setName("nve-tracked2");
	nveErrorVec3.setName("nve-error3");
	nveVec3.setName("nve-tracked3");
}

void CCWSApplLayer::Statistics::recordScalars(cSimpleModule& module)
{
	module.recordScalar("Sent Updates", sentUpdates);
	module.recordScalar("Received Updates", receivedUpdates);
	module.recordScalar("Threshold Violations", thresholdViolations);
	module.recordScalar("Time Violations", timeViolations);
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
		// set autoregressive model parameters
		xModel.set(0.9, 0.436, 0.2);
		yModel.set(0.9, 0.436, 0.2);
		speedModel.set(0.9, 0.436, 0.2);
		angleModel.set(0.9, 0.436, 0.0174532925);

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
			if (simTime() >= simulation.getWarmupPeriod())
				stats.receivedUpdates++;

			// record distance to tracked vehicle
			distance = spe.getPosition().distance(Coord(m->getX(), m->getY()));
			stats.nveDistanceVec.record(distance);

			// record latency to receive packet
			stats.nveLatencyVec.record(simTime() - m->getUtc());

			// if no NVE exists create one
			from = m->getSrcAddr();
			if (nve[from] == 0)
				nve[from] = new PositionEstimator();

			// update nve position
			nve[from]->updatePosition(m->getX(), m->getY(), m->getSpeed(), m->getAngleX(), m->getAngleY(), m->getAccel(), m->getCreationTime());

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
	    	if ((simTime() - rpe.getLastUpdated()) >= autoRetransmitTime)
	    	{
	    		if (simTime() >= simulation.getWarmupPeriod())
	    			stats.timeViolations++;
	    		send = true;
	    	}
	    	else if (errorSize > thresholdSize)
			{
	    		if (simTime() >= simulation.getWarmupPeriod())
	    			stats.thresholdViolations++;
				send = true;
			}
			else if (errorSize > 0.5*thresholdSize)
			{
				delayTime /= 5;
			}
			else if (errorSize > 0.75*thresholdSize)
			{
				delayTime /= 10;
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
	double x, y, speed;

	if (simTime() >= runUp)
	{
		x = spe.getCurrentPosition(simTime()).getX() + xModel.getValue();
		y = spe.getCurrentPosition(simTime()).getY() + yModel.getValue();
		speed = spe.getSpeed() + speedModel.getValue();

		double newX = spe.getAngle().getX() * cos(angleModel.getValue()) - spe.getAngle().getY() * sin(angleModel.getValue());
		double newY = spe.getAngle().getY() * sin(angleModel.getValue()) + spe.getAngle().getY() * cos(angleModel.getValue());

		CCWSApplPkt *pkt = new CCWSApplPkt("CCWS_MESSAGE", CCWS_MESSAGE);
		pkt->setDestAddr(-1);

		// we use the host modules getIndex() as a appl address
		pkt->setSrcAddr(myApplAddr());
		pkt->setBitLength(headerLength);

		pkt->setAccel(spe.getAcceleration());
		pkt->setX(x);
		pkt->setY(y);
		pkt->setSpeed(speed);
		pkt->setAngleX(newX);
		pkt->setAngleY(newY);
		pkt->setBitLength(headerLength);
		pkt->setUtc(simTime());

		// set the control info to tell the network layer the layer 3
		// address;
		pkt->setControlInfo(new WAVEControlInfo(L2BROADCAST, CCH, SIXMBS, txPower) );

		if (debug) EV << "Sending broadcast location packet!   " << simTime() << endl ;

		if (simTime() >= simulation.getWarmupPeriod())
			stats.sentUpdates++;

		if (rpe.getNumberUpdates() > 1)
			stats.rpeTransmitVec.record(simTime() - rpe.getLastUpdated());

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

				// vision updates
				if (isRegistered)
				{
					if (visionOn)
					{
						vm->updateVehiclePos(myApplAddr(), &(m->getStartPos()), &(m->getDirection()));
						stats.visibleVec.record(vm->visible(myApplAddr()));
						stats.mvisibleVec.record(vm->maybeVisible(myApplAddr()));
					}
				}
				else
				{
					vm->registerVehicle(this, &(m->getStartPos()), &(m->getDirection()));
					isRegistered = true;
				}

				// update autoregressive model
				xModel.nextValue();
				yModel.nextValue();
				speedModel.nextValue();
				angleModel.nextValue();

				int count = 0;
				int count1 = 0;
				int count2 = 0;
				int count3 = 0;
				int deleted = 0;
				for (int i=0; i<maxVehicles; i++)
				{
					if (nve[i] != 0)
					{
						if ((simTime() - nve[i]->getLastUpdated()) <= nveTimeout && vm->vehicleExists(i))
						{
							double diff = nve[i]->positionError(vm->getVehiclePos(i), simTime());
							stats.nveErrorVec.record(diff);

							Coord pos = spe.getCurrentPosition();
							double distance = nve[i]->positionError(pos);

							if (distance < 20.0)
							{
								count1++;
								stats.nveErrorVec1.record(diff);
							}
							if (distance < 50.0)
							{
								count2++;
								stats.nveErrorVec2.record(diff);
							}
							else if (distance < 100.0)
							{
								count3++;
								stats.nveErrorVec3.record(diff);
							}

							count++;
						}
						else
						{
							delete nve[i];
							nve[i] = 0;
							deleted++;
						}
					}
				}
				stats.ndeletecVec.record(deleted);
				stats.nveVec.record(count);

				stats.nveVec1.record(count1);
				stats.nveVec2.record(count2);
				stats.nveVec3.record(count3);
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
