/*
 * TraCIMobilityV.cpp
 *
 *  Created on: 2011-03-22
 *      Author: Kyle
 */

#include "TraCIMobilityV.h"

Define_Module(TraCIMobilityV);

void TraCIMobilityV::initialize(int stage)
{
	TraCIMobility::initialize(stage);

	if (stage == 1)
	{
		location.setSpeed(0);
		location.setAngleRad(0);
		location.setAccel(0);
		detailedMoveCategory = utility->getCategory(&location);
	}

}

void TraCIMobilityV::changePosition()
{
	simtime_t updateInterval = simTime() - this->lastUpdate;
	location.setSpeed(speed);
	location.setAccel((speed - last_speed) / updateInterval);
	location.setAngleRad(angle);
	TraCIMobility::changePosition();

	//publish the the new move
	utility->publishBBItem(detailedMoveCategory, &location, hostId);
}
