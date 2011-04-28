/*
 * VisionEntry.h
 *
 *  Created on: 2011-03-29
 *      Author: Kyle
 */

#ifndef VISIONENTRY_H_
#define VISIONENTRY_H_

#include <omnetpp.h>

#include "Coord.h"
#include "MinMax.h"
#include "VisionMap.h"

class VisionEntry;

struct VisibleVehicle
{
	VisionEntry* vehicle;
	double distance;
	bool visible;
	MinMax angles;
};

typedef std::list<VisibleVehicle> VehicleList;

class VisionEntry
{
public:
	VisionEntry();

	double getAngleTo(VisionEntry* other);
	double getAngleTo(const Coord& second);

	double getDistanceTo(VisionEntry* other);

    MinMax getMinMaxAngles(VisionEntry* other);

    void pruneVisible(int cutoff);

	VehicleList withinRange;

	/** @brief module id of the nic for which information is stored*/
	int vehicleId;

	/** @brief Pointer to the application module */
	cModule *appPtr;

	int maybeVisible;
	int visible;

	/** @brief Geographic location of the nic*/
	Coord pos;

	Coord angle;

protected:

	double width;
	double length;

};

#endif /* VISIONENTRY_H_ */
