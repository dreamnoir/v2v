/*
 * VisionEntry.cpp
 *
 *  Created on: 2011-03-29
 *      Author: Kyle
 */

#include "VisionEntry.h"

#define RAD_TO_DEGREE 57.2957795

VisionEntry::VisionEntry(int length, int width, bool visionOn)
{
	this->visionOn = visionOn;
	this->possible = 0;
	this->visible = 0;
	this->width = width;
	this->length = length;
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

double VisionEntry::getDistanceTo(VisionEntry* other)
{
	Coord widthVec(other->angle.getY(), other->angle.getX());
	widthVec *= (other->width/2);
	Coord lengthVec = other->angle*(other->length/2);

	double distances [] =  {this->pos.distance(other->pos+lengthVec),
							this->pos.distance(other->pos-lengthVec),
							this->pos.distance(other->pos+widthVec),
							this->pos.distance(other->pos+widthVec),
							this->pos.distance(other->pos+lengthVec+widthVec),
							this->pos.distance(other->pos+lengthVec-widthVec),
							this->pos.distance(other->pos-lengthVec+widthVec),
							this->pos.distance(other->pos-length-widthVec)};

	double min = 1000000;

	for (int i=0; i<8; i++)
	{
		if (distances[i] < min)
			min = distances[i];
	}

	return min;
}

MinMax VisionEntry::getMinMaxAngles(VisionEntry* other)
{
	MinMax result = {10000, - 10000};

	Coord widthVec(other->angle.getY(), other->angle.getX());
	widthVec *= (other->width/2);
	Coord lengthVec = other->angle*(other->length/2);

	double angles [] = {getAngleTo(other->pos+lengthVec+widthVec),
						getAngleTo(other->pos+lengthVec-widthVec),
						getAngleTo(other->pos-lengthVec+widthVec),
						getAngleTo(other->pos-lengthVec-widthVec)};

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

void VisionEntry::pruneVisible(int cutoff)
{
	possible = withinRange.size();
	visible = 0;
	VisionMap m(cutoff, false);

	for (VehicleList::iterator ci = withinRange.begin(); ci != withinRange.end(); ci++)
	{

		//ev << "Visible: " << (*ci).vehicle->vehicleId << " at d=" << (*ci).distance << " and angle (" << (*ci).angles.min << "," << (*ci).angles.max << ")" << endl;

		if (!m.visible((*ci).angles))
		{
			(*ci).visible = false;
			ev << "Not actually visible" << endl;
		}
		else
		{
			visible++;
		}


		m.add((*ci).angles);
	}
}

