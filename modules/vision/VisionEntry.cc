/*
 * VisionEntry.cpp
 *
 *  Created on: 2011-03-29
 *      Author: Kyle
 */

#include "VisionEntry.h"

#define RAD_TO_DEGREE 57.2957795

VisionEntry::VisionEntry()
{
	visible = 0;
	width = 2;
	length = 5;
}

double VisionEntry::getAngleTo(const Coord& second)
{
	// get vector from this vehicle to other one, make into unit length and
	// find angle between it and direction of this vehicle
	Coord v1v2 = second - this->pos;
	v1v2 = v1v2 / v1v2.length();
	return acos(this->angle.getX()*v1v2.getX() + this->angle.getY()*v1v2.getY()) * RAD_TO_DEGREE;
}

double VisionEntry::getAngleTo(VisionEntry* other)
{
	return getAngleTo(other->pos);
}

MinMax VisionEntry::getMinMaxAngles(VisionEntry* other)
{
	MinMax result = {10000, - 10000};

	Coord widthVec(other->angle.getY(), other->angle.getX());
	widthVec *= (other->width/2);
	Coord lengthVec = other->angle*(length/2);

	double angles [] = {getAngleTo(other->pos+lengthVec+widthVec), getAngleTo(other->pos+lengthVec-widthVec), getAngleTo(other->pos-lengthVec+widthVec), getAngleTo(other->pos-lengthVec-widthVec)};

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

int VisionEntry::pruneVisible()
{
	MinMaxList list;
	for (VehicleList::iterator ci = withinRange.begin(); ci != withinRange.end(); ci++)
	{

		ev << "Visible: " << (*ci).vehicle->vehicleId << " at d=" << (*ci).distance << endl;

	}

	return visible;
}

