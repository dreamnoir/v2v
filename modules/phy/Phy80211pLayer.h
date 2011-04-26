#ifndef PHY80211PLAYER_H_
#define PHY80211PLAYER_H_

#include <PhyLayer.h>
#include "Decider80211p.h"

class Phy80211pLayer: public PhyLayer {
protected:
	virtual Decider* getDeciderFromName(std::string name, ParameterMap& params);

	/**
	 * @brief Initializes a new Decider80211 from the passed parameter map.
	 */
	virtual Decider* initializeDecider80211p(ParameterMap& params);
};
#endif /* PHY80211PLAYER_H_ */
