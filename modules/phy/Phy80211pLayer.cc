#include "Phy80211pLayer.h"

Define_Module(Phy80211pLayer);

Decider* Phy80211pLayer::getDeciderFromName(std::string name, ParameterMap& params)
{
	if(name == "Decider80211p") {
		protocolId = IEEE_80211;
		return initializeDecider80211p(params);
	}
	return PhyLayer::getDeciderFromName(name, params);
}

Decider* Phy80211pLayer::initializeDecider80211p(ParameterMap& params) {
	double threshold = params["threshold"];
	return new Decider80211p(this, threshold, sensitivity,
							radio->getCurrentChannel(),
							findHost()->getIndex(), coreDebug);
}
