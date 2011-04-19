/*
 * CCWSApplLayer.h
 *
 *  Created on: 2011-02-24
 *      Author: Kyle
 */

#ifndef CCWSAPPLLAYER_H_
#define CCWSAPPLLAYER_H_

#include "BaseApplLayer.h"
#include "PositionEstimator.h"
#include "VisionManager.h"

struct PositionInformation
{
	double x;
	double y;
	double angleX;
	double angley;
	double speed;
	double acceleration;
	double time;
	int txPower;
	int dataRate;
};

class CCWSApplLayer  : public BaseApplLayer
{
public:
	class Statistics {
		public:
			int sentUpdates;
			int receivedUpdates;
			int shortDelay;

			cOutVector speErrorVec;			// SPE error between position updates

			cOutVector nveErrorVec;			// NVE error between position updates
			cOutVector nveDistanceVec;		// NVE distance to other vehicle

			cOutVector visibleVec;			// vehicles in range of vision
			cOutVector mvisibleVec;
			cOutVector nveVec;				// vehicles tracked by nve
			cOutVector thresholdVec;		// time between threshold communication
			cOutVector ndeletecVec;

			void initialize();
			//void watch(cSimpleModule& module);
			void recordScalars(cSimpleModule& module);
	};

	virtual ~CCWSApplLayer();

	/** @brief Initialization of the module and some variables*/
	virtual void initialize(int);

	virtual void finish();

    /** @brief Message kinds used by this layer.*/
    enum TestApplMessageKinds{
		SEND_BROADCAST_TIMER = LAST_BASE_APPL_MESSAGE_KIND,
		CHECK_POSITION_UPDATE,
		RETRANSMIT_POSITION_UPDATE,
		CCWS_MESSAGE,
		EWS_MESSAGE,
		HWS_MESSAGE
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

	// how long before auto retransmit
	double autoRetransmitTime;

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

	// time before deleting unupdated nve estimator
	int nveTimeout;

	// max number of vehicles in simulation
	int maxVehicles;

};

#endif /* CCWSAPPLLAYER_H_ */
