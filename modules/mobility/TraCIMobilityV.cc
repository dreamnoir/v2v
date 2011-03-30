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

	//fix this, must be a radian issue?
	Coord vangle = new Coord(-1*cos(this->angle), -1*sin(this->angle));

	move.setSpeed(this->speed);
	move.setDirectionByVector(vangle);
	TraCIMobility::changePosition();
}
