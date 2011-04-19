#ifndef WAVETCONTROLINFO_H
#define WAVECONTROLINFO_H

#include "NetwControlInfo.h"

class WAVEControlInfo: public NetwControlInfo
{
  protected:
    int txPower;
    int channel;
    int dataRate;

  public:
    /** @brief Default constructor*/
    WAVEControlInfo(int addr = 0, int chan = 0, int rate = 0, int power = 0)
    {
    	netwAddr = addr;
    	channel = chan;
    	dataRate = rate;
    	txPower = power;
    }

    /** @brief Destructor*/
    virtual ~WAVEControlInfo( ){};

    /** @brief Getter method*/
	virtual const int getChannel() {return channel;};
	virtual void setChannel(const int chan) {channel = chan;};

	/** @brief Getter method*/
	virtual const int getDataRate() {return dataRate;};
	virtual void setDataRate(const int rate) {dataRate = rate;};

    /** @brief Getter method*/
    virtual const int getTxPower() {return txPower;};
    virtual void setTxPower(const int power) {txPower = power;};

};

#endif
