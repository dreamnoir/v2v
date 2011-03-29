/*
 * TraCIMobilityV.h
 *
 *  Created on: 2011-03-22
 *      Author: Kyle
 */

#ifndef TRACIMOBILITYV_H_
#define TRACIMOBILITYV_H_

#include "TraCIMobility.h"
#include "Move.h"

class TraCIMobilityV : public TraCIMobility{

	public:
		virtual void initialize(int stage);
		virtual void changePosition();
};

#endif /* TRACIMOBILITYV_H_ */
