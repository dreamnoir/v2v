/*
 * TimedApplLayer.h
 *
 *  Created on: 2011-02-24
 *      Author: Kyle
 */

#ifndef TIMEDAPPLLAYER_H_
#define TIMEDAPPLLAYER_H_

#include "BaseApplLayer.h"
#include "PositionEstimator.h"

class TimedApplLayer  : public BaseApplLayer
{
public:
	class Statistics {
		public:
			double sentUpdates; /**< for statistics: number of first road we encountered (if road id can be expressed as a number) */
			double receivedUpdates;

			void initialize();
			//void watch(cSimpleModule& module);
			void recordScalars(cSimpleModule& module);
	};

	virtual ~TimedApplLayer();



	/** @brief Initialization of the module and some variables*/
	virtual void initialize(int);

	virtual void finish();

    /** @brief Message kinds used by this layer.*/
    enum TestApplMessageKinds{
		SEND_BROADCAST_TIMER = LAST_BASE_APPL_MESSAGE_KIND,
		BROADCAST_MESSAGE,
		BROADCAST_REPLY_MESSAGE,
		LAST_TEST_APPL_MESSAGE_KIND
    };

protected:
    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

    /** @brief send a broadcast packet to all connected neighbors */
    void sendBroadcast();

    /** @brief send a reply to a broadcast message */
    void sendReply(ApplPkt *msg);

    virtual void receiveBBItem(int category, const BBItem *details, int scopeModuleId);

	/** @brief Timer message for scheduling next message.*/
	cMessage *delayTimer;

	Statistics stats;

	PositionEstimator spe;

	double delay;
	int catMove;

	cOutVector errorVec;

};

#endif /* TIMEDAPPLLAYER_H_ */
