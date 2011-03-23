#ifndef DETAILEDMOVE_H
#define DETAILEDMOVE_H

#include "Move.h"

class DetailedMove : public Move
{

	protected:
		double accel;
		double angleRad;

	public:

		double getAccel() const
		{
			return this->accel;
		}

		void setAccel(double accel)
		{
			this->accel = accel;
		}

		double getAngleRad()
		{
			return this->angleRad;
		}

		void setAngleRad(double angleRad)
		{
			this->angleRad = angleRad;
		}

	    std::string info() const {
	        std::ostringstream ost;
	        ost << " HostMove "
	            << " startPos: " << startPos.info()
	            << " direction: " << angleRad
	            << " acceleration: " << accel
	            << " startTime: " << startTime
	            << " speed: " << speed;
	        return ost.str();
	    }

};

#endif
