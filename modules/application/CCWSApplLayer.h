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
#include "AutoregModel.h"

struct VehicleInfo
{
	VisibleVehicle vehicle;
	int trackAs;
};

class VisionPositionEstimator
{
public:
	VisionPositionEstimator()
	{
		this->updated = simTime();
	}
	void updatePosition()
	{
		this->updated = simTime();
	}
	simtime_t getLastUpdated()
	{
		return updated;
	}
protected:
	simtime_t updated;
};

typedef std::list<VehicleInfo> VehicleTrackingList;

class CCWSApplLayer  : public BaseApplLayer
{
public:
	class Statistics {
		public:
			int sentUpdates;
			int receivedUpdates;

			double length;
			double width;

			int thresholdViolations;
			int timeViolations;

			cOutVector rpeTransmitVec;

			cOutVector nveErrorVec;			// NVE error between position updates
			cOutVector nveVec;				// vehicles tracked by nve
			cOutVector nveDistanceVec;		// NVE distance to other vehicle
			cOutVector nveLatencyVec;		// NVE latency to other vehicle

			cOutVector visibleVec;			// vehicles in range of vision
			cOutVector occludedVec;
			cOutVector visibleTrackedVec;
			cOutVector visibleNotTrackedVec;

			cOutVector unifiedError;
			cOutVector trackedWrong;
			cOutVector visibleMissed;

			cOutVector netwVision;
			cOutVector allTracked;
			//cOutVector unifiedDistanceError1;

			//cOutVector unifiedMinError2;
			//cOutVector unifiedMaxError2;
			//cOutVector unifiedDistanceError2;

			cOutVector thresholdVec;		// time between threshold communication
			cOutVector ndeletecVec;

			cOutVector extraCCWS;			// NVE error between position updates
			//cOutVector nveVec1;				// vehicles tracked by nve
			//cOutVector nveErrorVec2;			// NVE error between position updates
			//cOutVector nveVec2;				// vehicles tracked by nve
			//cOutVector nveErrorVec3;			// NVE error between position updates
			//cOutVector nveVec3;				// vehicles tracked by nve

			void initialize();
			//void watch(cSimpleModule& module);
			void recordScalars(cSimpleModule& module);
	};

	virtual ~CCWSApplLayer();

	/** @brief Initialization of the module and some variables*/
	virtual void initialize(int);

	virtual void finish();

	Coord getCurrentPos();

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

    MinMax getMinMaxAngles(PositionEstimate estimate, double length, double width);
    double getDistanceTo(PositionEstimate estimate, double length, double width);

    Coord getPositionGPS();
    double getSpeedGPS();
    Coord getHeadingGPS();

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

	VisionPositionEstimator** vpe;

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

	// how long before starting simulation
	double runUp;

	// max tx power to use
	int txPower;

	bool visionOn;

	int transmitPower;

	int bitrate;

	double length;
	double width;

	double adoptionRate;
	bool driverAssistance;

	AutoregModel xModel;
	AutoregModel yModel;
	AutoregModel speedModel;
	AutoregModel angleModel;

	VehicleTrackingList visible;

	bool extraCCWS;
};

#endif /* CCWSAPPLLAYER_H_ */
