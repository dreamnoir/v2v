/*
 * Mac80211p.h
 *
 *  Created on: 2011-03-23
 *      Author: Kyle
 */

#ifndef MAC80211P_H_
#define MAC80211P_H_

#include "Mac80211.h"

class Mac80211p : public Mac80211
{
	public:
		virtual void initialize(int stage);
		virtual void handleLowerControl(cMessage* msg);
		virtual void handleLowerMsg(cMessage *msg);

	    /** @brief send broadcast frame */
	    void sendBROADCASTframe();

	protected:
		cOutVector recVec;
		cOutVector notRecVec;
};

#endif /* MAC80211P_H_ */
