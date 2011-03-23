/*
 * TraCIMobilityV.h
 *
 *  Created on: 2011-03-22
 *      Author: Kyle
 */

#ifndef TRACIMOBILITYV_H_
#define TRACIMOBILITYV_H_

#include "TraCIMobility.h"
#include "DetailedMove.h"

class TraCIMobilityV : public TraCIMobility{

	protected:
		int	detailedMoveCategory;
		DetailedMove location;
	public:
		virtual void initialize(int stage);
		virtual void changePosition();
};

#endif /* TRACIMOBILITYV_H_ */
