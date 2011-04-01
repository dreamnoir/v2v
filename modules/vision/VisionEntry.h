/*
 * VisionEntry.h
 *
 *  Created on: 2011-03-29
 *      Author: Kyle
 */

#ifndef VISIONENTRY_H_
#define VISIONENTRY_H_

#include <omnetpp.h>
#include <map>

#include "Coord.h"

class VisionEntry;

struct MinMax
{
	double min;
	double max;

	bool operator<(const MinMax &other)
	{
		return other.max < min;
	}
	bool operator>(const MinMax &other)
	{
		return other.min > max;
	}
	bool inside(const MinMax &other)
	{
		return ((min < other.min && other.min > max) || (min < other.max && other.max > max));
	}
};

struct VisibleVehicle
{
	VisionEntry* vehicle;
	double distance;
	bool visible;
	MinMax angles;
};

typedef std::list<VisibleVehicle> VehicleList;
typedef std::list<MinMax> MinMaxList;

class VisionEntry
{
public:
	VisionEntry();

/** @brief Connect two nics */
	virtual void connectTo(VisionEntry*) {}

	/** @brief Disconnect two nics */
	virtual void disconnectFrom(VisionEntry*) {}

	virtual bool isConnected(VisionEntry*) {return false;}

	double getAngleTo(VisionEntry* other);
	double getAngleTo(const Coord& second);

    MinMax getMinMaxAngles(VisionEntry* other);

    int pruneVisible();

	VehicleList withinRange;

	/** @brief module id of the nic for which information is stored*/
	int vehicleId;

	/** @brief Pointer to the application module */
	cModule *appPtr;

	int visible;

	/** @brief Geographic location of the nic*/
	Coord pos;

	Coord angle;

protected:

	double width;
	double length;

};

#endif /* VISIONENTRY_H_ */
