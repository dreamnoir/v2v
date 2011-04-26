#ifndef NETWTO80211PCONTROLINFO_H
#define NETWTO80211PCONTROLINFO_H

#include "NetwToMacControlInfo.h"

class NetwTo80211pControlInfo : public NetwToMacControlInfo
{
  protected:
    int accessClass;

  public:

    enum
    {
		A3 = 0,
		A2,
		A1,
		A0
    };

    /** @brief Default constructor*/
    NetwTo80211pControlInfo(int addr = 0, int ac = 0) : NetwToMacControlInfo(addr)
    {

    	accessClass = ac;
    }

    /** @brief Destructor*/
    virtual ~NetwTo80211pControlInfo( ){};

    /** @brief Getter method*/
	virtual const int getAccessClass() {return accessClass;};
	virtual void setAccessClass(const int ac) {accessClass = ac;};

};

#endif
