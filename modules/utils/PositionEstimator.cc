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

#include "PositionEstimator.h"
#include "FWMath.h"

PositionEstimator::PositionEstimator()
{
	this->speed = 0;
	this->acceleration = 0;
	this->lastUpdated = simTime();
}

Coord PositionEstimator::getCurrentPosition(simtime_t time)
{
	double interval;
	double distance;

	//time interval since position updated
	interval = SIMTIME_DBL(simTime()) - SIMTIME_DBL(this->lastUpdated);

	//distance travelled during time interval
	distance = this->speed * interval + 0.5 * this->acceleration * pow(interval, 2);

	return this->position + this->angle*distance;
}

void PositionEstimator::updatePosition(const Coord& position, double speed, const Coord& angle)
{
	simtime_t updateInterval = simTime() - this->lastUpdated;

	//calculate acceleration before we update speed
	this->acceleration = (speed - this->speed) / updateInterval;
	this->position = position;
	this->speed = speed;
	this->angle = angle;
	this->lastUpdated = simTime();
}

double PositionEstimator::getSpeed()
{
	double interval = SIMTIME_DBL(simTime()) - SIMTIME_DBL(this->lastUpdated);
	return this->speed + this->acceleration*interval;
}
