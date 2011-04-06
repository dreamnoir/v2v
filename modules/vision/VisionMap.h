//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

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
