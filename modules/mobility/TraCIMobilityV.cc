/*
 * TraCIMobilityV.cpp
 *
 *  Created on: 2011-03-22
 *      Author: Kyle
 */

#include "TraCIMobilityV.h"
#include "FWMath.h"

Define_Module(TraCIMobilityV);

void TraCIMobilityV::initialize(int stage)
{
	TraCIMobility::initialize(stage);
}

void TraCIMobilityV::changePosition()
{
	Coord vangle = new Coord(cos(this->angle), sin(this->angle));

	move.setSpeed(this->speed);
	move.setDirectionByVector(vangle);
	TraCIMobility::changePosition();
}
