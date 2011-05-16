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
	int id;
	VisionEntry* vehicle;
	double distance;
	bool visible;
	MinMax angles;
};

typedef std::list<VisibleVehicle> VehicleList;
typedef std::list<int, VisibleVehicle> VehicleMapList;

class VisionEntry
{
public:
	VisionEntry(int length, int width);

	double getAngleTo(VisionEntry* other);
	double getAngleTo(const Coord& second);

	double getDistanceTo(VisionEntry* other);

    MinMax getMinMaxAngles(VisionEntry* other);

    void pruneVisible(int cutoff);

	VehicleList withinRange;

	/** id of the node this entry represents */
	int vehicleId;

	/** Pointer to the application module */
	cModule *appPtr;

	int maybeVisible;
	int visible;

	/** Geographic location of the node */
	Coord pos;

	// unit vector the node is pointing in
	Coord angle;

	double getWidth() {return width;}
	double getLength() {return length;}

protected:

	double width;
	double length;

};

#endif /* VISIONENTRY_H_ */
