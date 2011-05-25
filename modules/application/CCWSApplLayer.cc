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

void CCWSApplLayer::Statistics::initialize()
{
	sentUpdates = 0;
	receivedUpdates = 0;
	thresholdViolations = 0;
	timeViolations = 0;
	length = 0;
	width = 0;

	vehicleIdentified = -1;
	receivedMessage = -1;

	// setup vector staticis
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


	unifiedError.setName("unified-error");
	trackedWrong.setName("tracked-wrong");
	visibleMissed.setName("visible-missed");

	netwVision.setName("tracked-netw-vision");
	allTracked.setName("all-tracked");
	extraCCWS.setName("extra-ccws");

	vehicleIdentifiedVec.setName("vehicle-identified");
	wsSentVec.setName("ws-sent");

	changedMatch.setName("changed-match");

	//unifiedMinError2.setName("unified-min-error2");
	//unifiedMaxError2.setName("unified-max-error2");
	//unifiedDistanceError2.setName("unified-distance-error2");

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
	module.recordScalar("Received EWS/HWS Message", receivedMessage);
	module.recordScalar("Identified EWS/HWS Vehicle", vehicleIdentified);
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
		adoptionRate = par("adoptionRate").doubleValue();
		extraCCWS = par("extraCCWS").boolValue();
		sendEWS = par("sendEWS").doubleValue();
		sendHWS = par("sendHWS").doubleValue();
		hwsInterval = par("hwsInterval").doubleValue();
		ewsRepeat = par("ewsRepeat").doubleValue();
		ewsRepeatCount = par("ewsRepeatCount").doubleValue();
		ewsToHws = par("ewsToHws").boolValue();

		watchFor = -1;

		receivedMessage = 0;
		vehicleIdentified = 0;

		// make sure adoption rate is a percentage
		if (adoptionRate < 0)
			adoptionRate = 0;
		else if (adoptionRate > 1.0)
			adoptionRate = 1.0;

		// find vision manager
		vm = dynamic_cast<VisionManager*>(simulation.getModuleByPath("vision"));

		// not registered with vision manager yet
		isRegistered = false;

		// initialize statics module
		stats.initialize();


		// subscribe to movement updates
		Move moveBBItem;
		catMove = utility->subscribe(this, &moveBBItem, findHost()->getId());

		hwsTimer = 0;
		ewsTimer = 0;
		timer = 0;

	}
	else if(stage==1)
	{
		if (adoptionRate == 1.0)
			driverAssistance = true;
		else
			driverAssistance = (dblrand() < adoptionRate);

		if (driverAssistance)
		{
			// how long to wait before startin any simulation
			double add = 0;
			if (runUp > SIMTIME_DBL(simTime()))
				add = runUp - SIMTIME_DBL(simTime());

			// create and initialise empty array of NVE
			nve = new PositionEstimator*[maxVehicles];
			for (int i=0; i<maxVehicles; i++)
				nve[i] = 0;

			// create and initialise empty array of NVE
			vpe = new VisionPositionEstimator*[maxVehicles];
			for (int i=0; i<maxVehicles; i++)
				vpe[i] = 0;

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

			if (sendEWS != 0)
			{
				ewsTimer = new cMessage( "ews-timer", SEND_EWS_MESSAGE);
				scheduleAt(sendEWS, ewsTimer);
			}
			if (sendHWS != 0)
			{
				hwsTimer = new cMessage( "hws-timer", SEND_HWS_MESSAGE);
				scheduleAt(sendHWS, hwsTimer);
			}
		}
	}
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

void CCWSApplLayer::handleLowerMsg(cMessage* msg)
{
	int from;
	Coord estimate, now;
	double distance;
    CCWSApplPkt *m;
    WAVEControlInfo* control;

    if (driverAssistance)
    {
    	switch( msg->getKind() )
		{
			case CCWS_MESSAGE:

				// get packet and control information
				m = static_cast<CCWSApplPkt *>(msg);
				control = (WAVEControlInfo*) m->removeControlInfo();

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

				if (extraCCWS)
				{
					int ids[4];
					ids[0] = m->getId1();
					ids[1] = m->getId2();
					ids[2] = m->getId3();
					ids[3] = m->getId4();

					for (int i=0; i<4; i++)
					{
						if (ids[i] != -1 && nve[ids[i]] == 0)
						{
							if(vpe[ids[i]] == 0)
								vpe[ids[i]] = new VisionPositionEstimator();
							else
								vpe[ids[i]]->updatePosition();
						}
					}
				}
				break;

			case HWS_MESSAGE:
			case EWS_MESSAGE:

				if (watchFor == -1)
				{
					m = static_cast<CCWSApplPkt *>(msg);
					watchFor = m->getSrcAddr();
					receivedMessage = simTime();
					stats.receivedMessage = SIMTIME_DBL(simTime());
				}

				break;
			default:
				if (debug) EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;

		}
    }
    delete msg;
}

void CCWSApplLayer::handleSelfMsg(cMessage *msg)
{
	double errorSize, delayTime;
	Coord current;
	bool send = false;

	if (driverAssistance)
	{
		switch(msg->getKind())
		{
			case SEND_HWS_MESSAGE:
				if (timer != 0)
				{
					cancelAndDelete(timer);
					timer = 0;
				}

				sendPacketHWS();

				hwsTimer = new cMessage( "hws-timer", SEND_HWS_MESSAGE);
				scheduleAt(simTime() + hwsInterval, hwsTimer);

				break;

			case SEND_EWS_MESSAGE:
				if (timer != 0)
				{
					cancelAndDelete(timer);
					timer = 0;
				}
				sendPacketEWS();

				ewsTimer = 0;
				if (ewsRepeatCount > 0)
				{
					ewsTimer = new cMessage( "ews-timer", SEND_EWS_MESSAGE);
					scheduleAt(simTime() + ewsRepeat, ewsTimer);
					ewsRepeatCount--;
				}
				else if(ewsToHws)
				{
					hwsTimer = new cMessage( "hws-timer", SEND_HWS_MESSAGE);
					scheduleAt(simTime() + hwsInterval, hwsTimer);
				}

				break;

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
	}
	delete msg;
}


Coord CCWSApplLayer::getPositionGPS()
{
	Coord position = spe.getCurrentPosition();
	return position+Coord(xModel.getValue(), yModel.getValue());
}

Coord CCWSApplLayer::getHeadingGPS()
{
	double newX, newY;
	newX = spe.getAngle().getX() * cos(angleModel.getValue()) - spe.getAngle().getY() * sin(angleModel.getValue());
	newY = spe.getAngle().getY() * sin(angleModel.getValue()) + spe.getAngle().getY() * cos(angleModel.getValue());
	return Coord(newX, newY);
}

double CCWSApplLayer::getSpeedGPS()
{
	return spe.getSpeed() + speedModel.getValue();
}

void CCWSApplLayer::sendPacketEWS()
{
	stats.wsSentVec.record(2);
	CCWSApplPkt *pkt = new CCWSApplPkt("EWS_MESSAGE", EWS_MESSAGE);
	pkt->setDestAddr(-1);
	pkt->setSrcAddr(myApplAddr());
	pkt->setBitLength(headerLength);
	pkt->setControlInfo(new WAVEControlInfo(L2BROADCAST, CCH, bitrate, txPower) );
	sendDown(pkt);
}

void CCWSApplLayer::sendPacketHWS()
{
	stats.wsSentVec.record(1);
	CCWSApplPkt *pkt = new CCWSApplPkt("HWS_MESSAGE", HWS_MESSAGE);
	pkt->setDestAddr(-1);
	pkt->setSrcAddr(myApplAddr());
	pkt->setBitLength(headerLength);
	pkt->setControlInfo(new WAVEControlInfo(L2BROADCAST, CCH, bitrate, txPower) );
	sendDown(pkt);
}

void CCWSApplLayer::sendLocationUpdate()
{
	Coord position, heading;
	double speed;

	if (simTime() >= runUp && driverAssistance)
	{
		// get GPS position with GPS error added
		position = getPositionGPS();
		speed = getSpeedGPS();
		heading = getHeadingGPS();

		// create new CCWS packet
		CCWSApplPkt *pkt = new CCWSApplPkt("CCWS_MESSAGE", CCWS_MESSAGE);
		pkt->setDestAddr(-1);
		pkt->setSrcAddr(myApplAddr());
		pkt->setBitLength(headerLength);

		//set fields relating to vehicle position and trajectory
		pkt->setAccel(spe.getAcceleration());
		pkt->setX(position.getX());
		pkt->setY(position.getY());
		pkt->setSpeed(speed);
		pkt->setAngleX(heading.getX());
		pkt->setAngleY(heading.getY());
		pkt->setLength(length);
		pkt->setWidth(width);
		pkt->setUtc(simTime());

		if (extraCCWS)
		{
			int pos = 0;
			int ids[4] = {-1, -1, -1, -1};
			for (VehicleTrackingList::iterator ci = visible.begin(); ci != visible.end() && pos < 4; ci++)
			{
				if ((*ci).trackAs == -1)
				{
					ids[pos++] = (*ci).vehicle.id;
				}
			}

			pkt->setId1(ids[0]);
			pkt->setId2(ids[1]);
			pkt->setId3(ids[2]);
			pkt->setId4(ids[3]);

			stats.extraCCWS.record(pos);

			pkt->setBitLength(headerLength+320*pos);
		}

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
		// get movement information
		const Move* m = static_cast<const Move*>(details);

		if (m->getStartPos().getX() != 0 && m->getStartPos().getY() != 0)
		{
			// update SPE
			spe.updatePosition(m->getStartPos(), m->getSpeed(), m->getDirection());

			// vision updates
			if (isRegistered)
			{
				if (visionOn && simTime() > runUp)
				{
					vm->updateVehiclePos(appAddress, &(m->getStartPos()), &(m->getDirection()));
					if (driverAssistance)
					{
						stats.visibleVec.record(vm->visible(appAddress));
						stats.occludedVec.record(vm->maybeVisible(appAddress)-vm->visible(appAddress));
					}
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

				vm->registerVehicle(this, &(m->getStartPos()), &(m->getDirection()), length, width, driverAssistance);
				isRegistered = true;
			}

			if (driverAssistance && simTime() > runUp)
			{
				// update autoregressive model
				xModel.nextValue();
				yModel.nextValue();
				speedModel.nextValue();
				angleModel.nextValue();

				//get list
				visible.clear();
				VehicleList vis = vm->getVisible(appAddress);
				for (VehicleList::iterator ci = vis.begin(); ci != vis.end(); ci++)
				{
					if ((*ci).visible)
					{
						VehicleInfo add;
						add.vehicle = (*ci);
						add.trackAs = -1;
						add.error = 0;
						visible.push_back(add);
						if (watchFor != -1)
						{
							ev << "Is " << (*ci).id << " the same as " << watchFor << endl;
							if ((*ci).id == watchFor)
							{
								if (vehicleIdentified == 0)
								{
									vehicleIdentified = simTime();
									stats.vehicleIdentified = SIMTIME_DBL(simTime());
								}

								stats.vehicleIdentifiedVec.record(vehicleIdentified - receivedMessage);
							}
						}
					}
				}
				vis.clear();

				// counters for tracked and deleted NVEs
				int tracked = 0;
				int deleted = 0;
				int visibleTracked = 0;
				int visibleNotTracked = 0;
				int visibleMissed = 0;
				int trackedWrong = 0;

				for (int i=0; i<maxVehicles; i++)
				{
					if (nve[i] != 0)
					{
						double time = SIMTIME_DBL(simTime() - nve[i]->getLastUpdated());
						if (time <= nveTimeout && vm->vehicleExists(i))
						{
							PositionEstimate estimate = nve[i]->getPositionEstimate();
							bool isVisible = false;
							double vLength = vm->getLength(i);
							double vWidth = vm->getWidth(i);

							for (VehicleTrackingList::iterator ci = visible.begin(); ci != visible.end(); ci++)
							{
								VisibleVehicle v = (*ci).vehicle;

								double distance = getDistanceTo(estimate, vLength, vWidth);
								double ddiff = fabs(distance-v.distance);

								if (ddiff < 1.0)
								{
									// this allows for 1m of movement at 15m distance scaled
									double angleError = 57/distance;


									MinMax a = getMinMaxAngles(estimate, vLength, vWidth);
									double max = fabs(a.max-v.angles.max);
									double min = fabs(a.min-v.angles.min);

									if (max < angleError && min < angleError)
									{
										if ((*ci).trackAs == -1 || max+min+ddiff < (*ci).error)
										{
											if ((*ci).trackAs != -1)
											{
												ev << "FOUND BETTER MATCH with error of " << max+min+ddiff << " < "<< (*ci).error << endl;
												stats.changedMatch.record(i);
												if ((*ci).trackAs != v.id)
													trackedWrong--;
											}
											isVisible = true;
											visibleTracked++;
											if (v.id != i)
												trackedWrong++;
											(*ci).trackAs = i;
											(*ci).error = max+min+ddiff;
											stats.unifiedError.record(0);
										}
									}
								}
							}

							// nve stuff
							if (!isVisible)
							{
								diff = nve[i]->positionError(vm->getVehiclePos(i), simTime());
								stats.nveErrorVec.record(diff);
								stats.unifiedError.record(diff);
							}
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

				int trackedVisionNetw = 0;
				if (extraCCWS)
				{
					for (int i=0; i<maxVehicles; i++)
					{
						if (vpe[i] != 0)
						{
							if (simTime() - vpe[i]->getLastUpdated() <= nveTimeout && nve[i] == 0)
							{
								trackedVisionNetw++;
							}
							else
							{
								delete vpe[i];
								vpe[i] = 0;
							}
						}
					}
				}


				for (VehicleTrackingList::iterator ci = visible.begin(); ci != visible.end(); ci++)
				{
					if ((*ci).trackAs == -1)
					{
						if (nve[(*ci).vehicle.id] == 0)
							visibleNotTracked++;
						else
							visibleMissed++;
					}
				}

				stats.ndeletecVec.record(deleted);
				stats.nveVec.record(tracked);
				stats.visibleTrackedVec.record(visibleTracked);
				stats.visibleNotTrackedVec.record(visibleNotTracked);
				stats.trackedWrong.record(trackedWrong);
				stats.visibleMissed.record(visibleMissed);
				if (extraCCWS)
				{
					stats.netwVision.record(trackedVisionNetw);
					stats.allTracked.record(trackedVisionNetw+tracked);
				}

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

	if (driverAssistance)
	{
		// delete array of NVEs
		delete [] nve;
		delete [] vpe;
	}


	// cancel any timers
	if (timer != 0)
		cancelAndDelete(timer);
	if (hwsTimer != 0)
		cancelAndDelete(hwsTimer);
	if (ewsTimer != 0)
		cancelAndDelete(ewsTimer);
}
