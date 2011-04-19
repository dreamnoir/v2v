/*
 * WSMNetwLayer.h
 *
 *  Created on: 2011-03-15
 *      Author: Kyle
 */

#ifndef WSMNETWLAYER_H_
#define WSMNETWLAYER_H_

#include "WSMPkt_m.h"
#include "BaseNetwLayer.h"

class WSMNetwLayer : public BaseNetwLayer
{
public:

	enum WSMMessageKinds
	{
		CCWS_MESSAGE_KIND = BaseNetwLayer::LAST_BASE_NETW_MESSAGE_KIND,
	};
	void initialize(int stage);

	virtual void handleLowerControl(cMessage* msg);

    virtual cMessage* decapsMsg(NetwPkt*);

    virtual NetwPkt* encapsMsg(cPacket*);
};

#endif /* WSMNETWLAYER_H_ */
