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
#include "VisionManager.h"

class TimedApplLayer  : public BaseApplLayer
{
public:
	class Statistics {
		public:
			double sentUpdates; /**< for statistics: number of first road we encountered (if road id can be expressed as a number) */
			double receivedUpdates;
			int shortDelay;

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
		CHECK_POSITION_UPDATE,
		RETRANSMIT_POSITION_UPDATE,
		BROADCAST_MESSAGE,
    };

protected:
    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

    /** @brief send a broadcast packet to all connected neighbors */
    void sendLocationUpdate();

    virtual void receiveBBItem(int category, const BBItem *details, int scopeModuleId);

	// General purpose timer
	cMessage *timer;

	// handles vision tasks
	VisionManager* vm;

	// handles statistics collection
	Statistics stats;

	// is the class registered with the VisionManager
	bool isRegistered;

	// self position estimator
	PositionEstimator spe;

	// remote position estimator
	PositionEstimator rpe;

	// array of NVE for any neighbours that come within range
	PositionEstimator** nve;

	// delay between messages in timed mode
	double delay;

	// timed mode or threshold mode
	bool thresholdMode;

	// error threshold between SPE and RPE before new message is sent
	double thresholdSize;

	// retransmit position updates
	bool retransmit;

	// how long to wait before retransmit
	double retransmitTime;

	// category of Move class notices
	int catMove;

	// SPE error between position updates
	cOutVector errorVec;

	// NVE error between position updates
	cOutVector nerrorVec;

	// vehicles in range of vision
	cOutVector visibleVec;

	// vehicles tracked by nve
	cOutVector nveVec;

	// time between threshold communication
	cOutVector thresholdVec;

	// max number of vehicles in simulation
	int maxVehicles;

	simsignal_t nveSignalId;
	simsignal_t testId;

};

#endif /* TIMEDAPPLLAYER_H_ */
