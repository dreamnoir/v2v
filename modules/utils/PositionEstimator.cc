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
	this->updates = 0;
	this->speed = 0;
	this->acceleration = 0;
	this->lastUpdated = 0;
}

Coord PositionEstimator::getCurrentPosition(simtime_t time)
{
	double interval;
	double distance;

	//time interval since position updated
	interval = SIMTIME_DBL(simTime() - this->lastUpdated);

	//distance travelled during time interval
	distance = this->speed * interval + 0.5 * this->acceleration * interval*interval;

	return this->position + this->angle*distance;
}

void PositionEstimator::updatePosition(const Coord& position, double speed, const Coord& angle)
{
	if (updates == 0)
	{
		this->acceleration = 0;
	}
	else
	{
		simtime_t updateInterval = simTime() - this->lastUpdated;
		this->acceleration = (speed - this->speed) / updateInterval;
	}
	this->position = position;
	this->speed = speed;
	this->angle = angle;
	this->lastUpdated = simTime();
	this->updates++;
}

PositionEstimate PositionEstimator::getPositionEstimate()
{
	PositionEstimate result;

	result.pos = getCurrentPosition();
	result.angle = getAngle();
	result.speed = getSpeed();
	result.acceleration = getAcceleration();
	result.time = simTime();

	return result;
}

void PositionEstimator::updatePosition(double x, double y, double speed, double angleX, double angleY, double accel, simtime_t time)
{

	this->acceleration = accel;
	this->position.setX(x);
	this->position.setY(y);
	this->speed = speed;
	this->angle.setX(angleX);
	this->angle.setY(angleY);
	this->lastUpdated = time;
	this->updates++;
}

double PositionEstimator::positionError(const Coord& newPosition, simtime_t time)
{
	return newPosition.distance(this->getCurrentPosition(time));
}

double PositionEstimator::getSpeed()
{
	double interval = SIMTIME_DBL(simTime()) - SIMTIME_DBL(this->lastUpdated);
	return this->speed + this->acceleration*interval;
}
