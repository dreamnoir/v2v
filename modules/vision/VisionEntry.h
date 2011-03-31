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

class VisionEntry
{
public:
	VisionEntry();

/** @brief Connect two nics */
	virtual void connectTo(VisionEntry*) {}

	/** @brief Disconnect two nics */
	virtual void disconnectFrom(VisionEntry*) {}

	virtual bool isConnected(VisionEntry*) {return false;}

	std::list<VisionEntry*> withinRange;

	/** @brief module id of the nic for which information is stored*/
	int vehicleId;

	/** @brief Pointer to the application module */
	cModule *appPtr;

	int inRange;
	int visible;

	/** @brief Geographic location of the nic*/
	Coord pos;

	Coord angle;

};

#endif /* VISIONENTRY_H_ */
