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

#ifndef POSITIONESTIMATOR_H_
#define POSITIONESTIMATOR_H_

#include "Coord.h"

class PositionEstimator : public cObject{
public:
	PositionEstimator();

	//gets estimate of current position
	Coord getCurrentPosition(simtime_t time);

	//update position information
	int updatePosition(const Coord& position, double speed, const Coord& angle);
	int updatePosition(double x, double y, double speed, double angleX, double angleY, simtime_t time);

	//get error between provided position and expected position
	double positionError(const Coord& newPosition, simtime_t time);

	//get current estimated speed
	double getSpeed();

	//get number of updates
	int getNumberUpdates() {return updates;}

	double getAcceleration() {return this->acceleration;}
	const Coord& getAngle() {return this->angle;}
	const Coord& getPosition() {return this->position;}
	simtime_t getLastUpdated() {return this->lastUpdated;}

protected:

	// how many times has this been updated
	int updates;

	Coord position;
	double speed;
	double acceleration;
	Coord angle;
	simtime_t lastUpdated;
};

#endif /* POSITIONESTIMATOR_H_ */
