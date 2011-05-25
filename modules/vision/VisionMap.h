#ifndef VISIONMAP_H_
#define VISIONMAP_H_

#include <omnetpp.h>
#include "MinMax.h"

typedef std::list<MinMax> MinMaxList;

class VisionMap {
public:

	VisionMap(double cutoff, bool debug);

	// add angle to vision map
	void add(MinMax angles);

	// check if angle is visible to vision map
	bool visible(MinMax angles);

private:

	// does actual work of adding angles
	void addAngle(MinMax angles);

	// does actual work of checking vision range
	double visibleRange(MinMax angles);

	// print debug messages
	bool debug;

	// how much of a vehicle must be visible to count
	double cutoff;

	// list of angles obstructed
	MinMaxList list;

};

#endif /* VISIONMAP_H_ */
