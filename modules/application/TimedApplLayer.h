/*
 * TimedApplLayer.h
 *
 *  Created on: 2011-02-24
 *      Author: Kyle
 */

#ifndef TIMEDAPPLLAYER_H_
#define TIMEDAPPLLAYER_H_

#include "TestApplLayer.h"
//#include "ImNotifiable.h"

class TimedApplLayer  : public TestApplLayer
{
public:


	class Statistics {
		public:
			double sentUpdates; /**< for statistics: number of first road we encountered (if road id can be expressed as a number) */
			double receivedUpdates;

			void initialize();
			//void watch(cSimpleModule& module);
			//void recordScalars(cSimpleModule& module);
	};
/** @brief Initialize module parameters*/
  virtual void initialize(int);
  virtual void handleLowerMsg(cMessage* msg);
  virtual void receiveBBItem(int category, const BBItem *details, int scopeModuleId);

protected:
  double delay;
  virtual void handleSelfMsg(cMessage*);
  int catMove;
  cOutVector tVec;
  cOutVector recVec;
  cOutVector failedVec;

};

#endif /* TIMEDAPPLLAYER_H_ */
