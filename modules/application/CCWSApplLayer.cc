#include "CCWSApplLayer.h"

#include "WAVEAddress.h"
#include "WAVEControlInfo.h"

#include "Move.h"
#include "CCWSApplPkt_m.h"
#include "Coord.h"

#include "TraCIMobilityV.h"
#include "TraCIScenarioManager.h"
#include "FindModule.h"

#include "MinMax.h"
#include "VisionManager.h"

#define RAD_TO_DEGREE 57.2957795

Define_Module(CCWSApplLayer);

double getAngleTo(const Coord& pos, const Coord& angle, const Coord& to)
{
	// get vector from this vehicle to other one, make into unit length and
	// find angle between it and direction of this vehicle
	Coord v1v2 = to - pos;
	v1v2 /= v1v2.length();
	return acos(angle.getX()*v1v2.getX() + angle.getY()*v1v2.getY()) * RAD_TO_DEGREE;
}

MinMax CCWSApplLayer::getMinMaxAngles(PositionEstimate estimate, double length, double width)
{
	MinMax result = {10000, - 10000};

	Coord widthVec(estimate.angle.getY(), estimate.angle.getX());
	widthVec *= (width/2);
	Coord lengthVec = estimate.angle*(length/2);

	Coord pos = spe.getCurrentPosition();
	Coord angle = spe.getAngle();

	double angles [] = {getAngleTo(pos, angle, estimate.pos+lengthVec+widthVec),
						getAngleTo(pos, angle, estimate.pos+lengthVec-widthVec),
						getAngleTo(pos, angle, estimate.pos-lengthVec+widthVec),
						getAngleTo(pos, angle, estimate.pos-lengthVec-widthVec)};

	for (int i=0; i<4; i++)
	{
		if (angles[i] < result.min)
			result.min = angles[i];
		if (angles[i] > result.max)
			result.max = angles[i];
	}

	if (result.max-result.min > 180)
	{
		double temp = result.max;
		result.max = result.min;
		result.min = temp;
	}

	return result;
}

double CCWSApplLayer::getDistanceTo(PositionEstimate estimate, double length, double width)
{
	Coord widthVec(estimate.angle.getY(), estimate.angle.getX());
	widthVec *= (width/2);
	Coord lengthVec = estimate.angle*(length/2);

	Coord pos = spe.getCurrentPosition();

	double distances [] =  {pos.distance(estimate.pos+lengthVec),
							pos.distance(estimate.pos-lengthVec),
							pos.distance(estimate.pos+widthVec),
							pos.distance(estimate.pos+widthVec),
							pos.distance(estimate.pos+lengthVec+widthVec),
							pos.distance(estimate.pos+lengthVec-widthVec),
							pos.distance(estimate.pos-lengthVec+widthVec),
							pos.distance(estimate.pos-length-widthVec)};

	double min = 1000000;

	for (int i=0; i<8; i++)
	{
		if (distances[i] < min)
			min = distances[i];
	}

	return min;
}

void CCWSApplLayer::Statistics::initialize()
{
	sentUpdates = 0;
	receivedUpdates = 0;
	thresholdViolations = 0;
	timeViolations = 0;
	length = 0;
	width = 0;

	// setup vector staticis
	speErrorVec.setName("spe-error");
	rpeTransmitVec.setName("rpe-interval");
	nveErrorVec.setName("nve-error");
	nveDistanceVec.setName("nve-distance");
	nveLatencyVec.setName("nve-latency");
	visibleVec.setName("visible");
	occludedVec.setName("occluded");
	thresholdVec.setName("threshold-error");
	nveVec.setName("nve-tracked");
	ndeletecVec.setName("nve-deleted");
	visibleTrackedVec.setName("visible-tracked");
	visibleNotTrackedVec.setName("visible-not-tracked");

	unifiedMinError.setName("unified-min-error");
	unifiedMaxError.setName("unified-max-error");
	unifiedDistanceError.setName("unified-distance-error");

	unifiedMinError1.setName("unified-min-error1");
	unifiedMaxError1.setName("unified-max-error1");
	unifiedDistanceError1.setName("unified-distance-error1");

	unifiedMinError2.setName("unified-min-error2");
	unifiedMaxError2.setName("unified-max-error2");
	unifiedDistanceError2.setName("unified-distance-error2");

	//nveErrorVec1.setName("nve-error1");
	//nveVec1.setName("nve-tracked1");
	//nveErrorVec2.setName("nve-error2");
	//nveVec2.setName("nve-tracked2");
	//nveErrorVec3.setName("nve-error3");
	//nveVec3.setName("nve-tracked3");
}

void CCWSApplLayer::Statistics::recordScalars(cSimpleModule& module)
{
	module.recordScalar("Sent Updates", sentUpdates);
	module.recordScalar("Received Updates", receivedUpdates);
	module.recordScalar("Threshold Violations", thresholdViolations);
	module.recordScalar("Time Violations", timeViolations);
	module.recordScalar("Length", length);
	module.recordScalar("Width", width);
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
		length = 0;
		width = 0;

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
		bitrate = (int) par("bitrate").doubleValue();

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
		// how long to wait before startin any simulation
		double add = 0;
		if (runUp > SIMTIME_DBL(simTime()))
					add = runUp - SIMTIME_DBL(simTime());

		// create and initialise empty array of NVE
		nve = new PositionEstimator*[maxVehicles];
		for (int i=0; i<maxVehicles; i++)
			nve[i] = 0;

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
	double x, y, speed, newX, newY;

	if (simTime() >= runUp)
	{
		// get GPS position with GPS error added
		x = spe.getCurrentPosition(simTime()).getX() + xModel.getValue();
		y = spe.getCurrentPosition(simTime()).getY() + yModel.getValue();
		speed = spe.getSpeed() + speedModel.getValue();
		newX = spe.getAngle().getX() * cos(angleModel.getValue()) - spe.getAngle().getY() * sin(angleModel.getValue());
		newY = spe.getAngle().getY() * sin(angleModel.getValue()) + spe.getAngle().getY() * cos(angleModel.getValue());

		// create new CCWS packet
		CCWSApplPkt *pkt = new CCWSApplPkt("CCWS_MESSAGE", CCWS_MESSAGE);
		pkt->setDestAddr(-1);
		pkt->setSrcAddr(myApplAddr());
		pkt->setBitLength(headerLength);

		//set fields relating to vehicle position and trajectory
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
		pkt->setControlInfo(new WAVEControlInfo(L2BROADCAST, CCH, bitrate, txPower) );

		// record location update as sent
		if (simTime() >= simulation.getWarmupPeriod())
			stats.sentUpdates++;

		// record time between updates
		if (rpe.getNumberUpdates() > 1)
			stats.rpeTransmitVec.record(simTime() - rpe.getLastUpdated());

		// update remote rpe
		rpe.updatePosition(spe.getCurrentPosition(simTime()), spe.getSpeed(), spe.getAngle());

		sendDown(pkt);
	}
}


void CCWSApplLayer::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
	double diff;
	int appAddress = myApplAddr();

	BaseModule::receiveBBItem(category, details, scopeModuleId);
	if(category == catMove)
	{
		if (simTime() >= runUp)
		{
			// get movement information
			const Move* m = static_cast<const Move*>(details);

			if (m->getStartPos().getX() != 0 && m->getStartPos().getY() != 0)
			{
				// how far off was our SPE estimate
				diff = spe.positionError(m->getStartPos(), simTime());
				if (spe.getNumberUpdates() > 1)
					stats.speErrorVec.record(diff);

				// update SPE
				spe.updatePosition(m->getStartPos(), m->getSpeed(), m->getDirection());

				// vision updates
				if (isRegistered)
				{
					if (visionOn)
					{
						vm->updateVehiclePos(appAddress, &(m->getStartPos()), &(m->getDirection()));
						stats.visibleVec.record(vm->visible(appAddress));
						stats.occludedVec.record(vm->maybeVisible(appAddress)-vm->visible(appAddress));
					}
				}
				else
				{
					TraCIMobilityV* mobility = FindModule<TraCIMobilityV*>::findSubModule(this->getParentModule());
					TraCIScenarioManager* manager = mobility->getManager();

					std::string id = manager->commandGetVehicleTypeId(mobility->getExternalId());

				    if (id == "p1" || id == "p2" || id == "p3" || id == "p4")
				    {
				    	length = 3.5;
				    	width = 1.8;
				    }
				    else if (id == "p5" || id == "p6")
				    {
				    	length = 5;
				    	width = 1.8;
				    }
				    else if (id == "p7" || id == "p8")
				    {
				    	length = 5;
				    	width = 2;
				    }
				    else if (id == "t1")
				    {
				    	length = 14;
				    	width = 2.5;
				    }
				    else
				    {
				    	length = 19;
				    	width = 2.5;
				    }

				    stats.length = length;
				    stats.width = width;

					vm->registerVehicle(this, &(m->getStartPos()), &(m->getDirection()), length, width);
					isRegistered = true;
				}

				// update autoregressive model
				xModel.nextValue();
				yModel.nextValue();
				speedModel.nextValue();
				angleModel.nextValue();

				// counters for tracked and deleted NVEs
				int tracked = 0;
				int deleted = 0;

				for (int i=0; i<maxVehicles; i++)
				{
					if (nve[i] != 0)
					{
						double time = SIMTIME_DBL(simTime() - nve[i]->getLastUpdated());
						if (time <= nveTimeout && vm->vehicleExists(i))
						{
							diff = nve[i]->positionError(vm->getVehiclePos(i), simTime());
							stats.nveErrorVec.record(diff);
							tracked++;
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
				stats.nveVec.record(tracked);

				int visibleTracked = 0;
				int visibleNotTracked = 0;

				// get visible
				VehicleList vis = vm->getVisible(appAddress);
				for (VehicleList::iterator ci = vis.begin(); ci != vis.end(); ci++)
				{
					VisibleVehicle v = (*ci);

					if (v.visible)
					{
						ev << "vehicle with id= " << v.id << " and angles (" << v.angles.min << ", " << v.angles.max << ") at d=" << v.distance << endl;

						if (nve[v.id] != 0)
						{
							visibleTracked++;

							PositionEstimate e = nve[v.id]->getPositionEstimate();
							MinMax a = getMinMaxAngles(e, v.vehicle->getLength(), v.vehicle->getWidth());
							double distance = getDistanceTo(e, v.vehicle->getLength(), v.vehicle->getWidth())-v.distance;
							double max = a.max-v.angles.max;
							double min = a.min-v.angles.min;

							ev << "estimate angles (" << min << ", " << max << ") at d=" << distance << endl;

							double time = SIMTIME_DBL(simTime() - nve[v.id]->getLastUpdated());

							if (time < 0.66)
							{
								stats.unifiedDistanceError.record(fabs(distance));
								stats.unifiedMinError.record(fabs(min));
								stats.unifiedMaxError.record(fabs(max));
							}
							else if (time >= 0.66 && time < 1.33)
							{
								stats.unifiedDistanceError1.record(fabs(distance));
								stats.unifiedMinError1.record(fabs(min));
								stats.unifiedMaxError1.record(fabs(max));
							}
							else
							{
								stats.unifiedDistanceError2.record(fabs(distance));
								stats.unifiedMinError2.record(fabs(min));
								stats.unifiedMaxError2.record(fabs(max));
							}
						}
						else
						{
							visibleNotTracked++;
						}

					}
				}

				stats.visibleTrackedVec.record(visibleTracked);
				stats.visibleNotTrackedVec.record(visibleNotTracked);
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
