/*
 * Decider80211.cc
 *
 *  Created on: 11.02.2009
 *      Author: karl wessel
 */

#include "Decider80211p.h"
#include <DeciderResult80211.h>
#include <Mac80211Pkt_m.h>

/* added by Sorin Cocorada sorin.cocorada@gmail.com*/
static const unsigned short comb_calc[15][15]=
{
    {    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    {    2,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    {    3,    3,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    {    4,    6,    4,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    {    5,   10,   10,    5,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    {    6,   15,   20,   15,    6,    1,    0,    0,    0,    0,    0,    0,    0,    0 },
    {    7,   21,   35,   35,   21,    7,    1,    0,    0,    0,    0,    0,    0,    0 },
    {    8,   28,   56,   70,   56,   28,    8,    1,    0,    0,    0,    0,    0,    0 },
    {    9,   36,   84,  126,  126,   84,   36,    9,    1,    0,    0,    0,    0,    0 },
    {   10,   45,  120,  210,  252,  210,  120,   45,   10,    1,    0,    0,    0,    0 },
    {   11,   55,  165,  330,  462,  462,  330,  165,   55,   11,    1,    0,    0,    0 },
    {   12,   66,  220,  495,  792,  924,  792,  495,  220,   66,   12,    1,    0,    0 },
    {   13,   78,  286,  715, 1287, 1716, 1716, 1287,  715,  286,   78,   13,    1,    0 },
    {   14,   91,  364, 1001, 2002, 3003, 3432, 3003, 2002, 1001,  364,   91,   14,    1 }
};

double fraction2dB(double fraction)
{
	return 10*log10(fraction);
}

double dB2fraction(double dB)
{
    return pow(10.0, (dB / 10));
}

static unsigned short comb(int n,int k)
{
//    ASSERT((n>=1) && (n<=14) (k>=1) && (k<=14));
    return comb_calc[n-1][k-1];
}


static double Pd(int d, double Pb)
{
    long double sum=0.0;
    int k;
    if (d%2 == 1)
    {
        for (k=(d+1)/2; k<=d; k++)
            sum+=comb(d,k)*pow(Pb,k)*pow(1.0-Pb, d-k);
    }
    else
    {
        sum=0.5*comb(d,d/2)*pow(Pb, d/2)*pow(1.0-Pb,d/2);
        for (k=d/2+1; k<=d; k++)
            sum+=comb(d,k)*pow(Pb,k)*pow(1.0-Pb, d-k);
    }
    //printf("prob=%f d=%d sum=%f \n",Pb,d, sum);
    return sum;

    //return pow(4*Pb*(1-Pb), d/2);
}


static double Pb(int rate, double prob)
{
    long double probc;
    switch (rate)
    {

    case 1:
        //11, 0, 38, 0, 193, 0, 1331, 0, 7275, 0, 40406, 0, 234969, 0, 1337714, 0, 7594819, 0, 433775588, 0,
        probc=11*Pd(10,prob) + 38*Pd(12,prob) + 193*Pd(14,prob);//+1331*Pd(16,prob);
        break;

    case 2:
        //1, 16, 48, 158, 642, 2435, 9174, 34701, 131533, 499312,
        probc=Pd(6,prob)+16*Pd(7,prob)+48*Pd(8,prob);//+158*Pd(9,prob)+642*Pd(10,prob)
        //+2435*Pd(11,prob)+ 9174*Pd(12,prob)+34701*Pd(13,prob)+131533*Pd(14,prob)+499312*Pd(15,prob);
        break;

    case 3:
        //(8, 31, 160, 892, 4512, 23297, 120976, 624304, 3229885, 16721329,
        probc=8*Pd(5,prob)+31*Pd(6,prob)+150*Pd(7,prob);//+892*Pd(8,prob)+4512*Pd(9,prob)
        //+23297*Pd(10,prob)+120976*Pd(11,prob)+624304*Pd(12,prob)+ 3229885*Pd(13,prob)+ 16721329*Pd(14,prob);
        break;
    default:
        ;
    }

    return probc;
}


static double ber_bpsk(double snir, double bandwidth, double bitrate, char channelModel)
{
    double y=snir*bandwidth/bitrate;

    if (channelModel=='r')//Rayleigh
        return 0.5*(1.0-sqrt(y/(1.0+y)));

    return 0.5*erfc(sqrt(y));//awgn
}

static double ber_qpsk(double snir, double bandwidth, double bitrate, char channelModel)
{
    double y=snir*bandwidth/bitrate;

    if (channelModel=='r')//Rayleigh
        return 0.5*(1.0-sqrt(y/(1.0+y)));
    return 0.5*erfc(sqrt(y));//awgn
}

static double ber_16qam(double snir, double bandwidth, double bitrate, char channelModel)
{
    double y=snir*bandwidth/bitrate;

    if (channelModel=='r')//Rayleigh
        return ( 5.0/8.0-3.0/8.0*sqrt(2.0*y/(5.0+2.0*y))-1.0/4.0*sqrt(18.0*y/(5.0+18.0*y)) );
    return ( 0.375*erfc(sqrt(0.4*y))+0.25*erfc(3.0*sqrt(0.4*y)) );//awgn

}

static double ber_64qam(double snir, double bandwidth, double bitrate, char channelModel)
{
    double y=snir*bandwidth/bitrate;

    if (channelModel=='r')//Rayleigh
        return ( 13.0/24.0-7.0/24.0*sqrt(y/(7.0+y))-1.0/4.0*sqrt(9.0*y/(7.0+9.0*y)) );
    return 7.0/24.0*erfc(sqrt(y/7.0))+0.25*erfc(3.0*sqrt(y/7.0));//awgn
}


/* end added by Sorin Cocorada sorin.cocorada@gmail.com*/

Decider80211p::Decider80211p(	DeciderToPhyInterface* phy,
							double threshold,
							double sensitivity,
							int channel,
							int myIndex,
							bool debug):
	BaseDecider(phy, sensitivity, myIndex, debug),
	snrThreshold(threshold)
{
	assert(1 <= channel);
	assert(channel  <= 14);
	centerFrequency = CENTER_FREQUENCIES[channel];
	channelModel = 'r';
}

simtime_t Decider80211p::processNewSignal(AirFrame* frame) {
	if(currentSignal.first != 0) {
		debugEV << "Already receiving another AirFrame!" << endl;
		return notAgain;
	}

	// get the receiving power of the Signal at start-time and center frequency
	Signal& signal = frame->getSignal();
	Argument start(DimensionSet::timeFreqDomain);
	start.setTime(signal.getSignalStart());
	start.setArgValue(Dimension::frequency_static(), centerFrequency);

	double recvPower = signal.getReceivingPower()->getValue(start);

	// check whether signal is strong enough to receive
	if ( recvPower < sensitivity )
	{
		debugEV << "Signal is to weak (" << recvPower << " < " << sensitivity
				<< ") -> do not receive." << endl;
		// Signal too weak, we can't receive it, tell PhyLayer that we don't want it again
		return notAgain;
	}

	// Signal is strong enough, receive this Signal and schedule it
	debugEV << "Signal is strong enough (" << recvPower << " > " << sensitivity
			<< ") -> Trying to receive AirFrame." << endl;

	currentSignal.first = frame;
	currentSignal.second = EXPECT_END;

	//channel turned busy
	setChannelIdleStatus(false);

	return ( signal.getSignalStart() + signal.getSignalLength() );
}

double Decider80211p::calcChannelSenseRSSI(simtime_t start, simtime_t end) {
	Mapping* rssiMap = calculateRSSIMapping(start, end);

	Argument min(DimensionSet::timeFreqDomain);
	min.setTime(start);
	min.setArgValue(Dimension::frequency_static(), centerFrequency - 5e6);
	Argument max(DimensionSet::timeFreqDomain);
	max.setTime(end);
	max.setArgValue(Dimension::frequency_static(), centerFrequency + 5e6);

	double rssi = MappingUtils::findMax(*rssiMap, min, max);

	delete rssiMap;

	return rssi;
}


DeciderResult* Decider80211p::checkIfSignalOk(AirFrame* frame)
{
	// check if the snrMapping is above the Decider's specific threshold,
	// i.e. the Decider has received it correctly

	// first collect all necessary information
	Mapping* snrMap = calculateSnrMapping(frame);
	assert(snrMap);

	Signal& s = frame->getSignal();
	simtime_t start = s.getSignalStart();
	simtime_t end = start + s.getSignalLength();

	start = start + RED_PHY_HEADER_DURATION; //its ok if the phy header is received only
											 //partly - TODO: maybe solve this nicer
	Argument min(DimensionSet::timeFreqDomain);
	min.setTime(start);
	min.setArgValue(Dimension::frequency_static(), centerFrequency - 5e6);
	Argument max(DimensionSet::timeFreqDomain);
	max.setTime(end);
	max.setArgValue(Dimension::frequency_static(), centerFrequency + 5e6);

	double snirMin = MappingUtils::findMin(*snrMap, min, max);

	EV << " snrMin: " << fraction2dB(snirMin) << endl;

	ConstMappingIterator* bitrateIt = s.getBitrate()->createConstIterator();
	bitrateIt->next(); //iterate to payload bitrate indicator
	double payloadBitrate = bitrateIt->getValue();
	delete bitrateIt;

	DeciderResult80211* result = 0;

	if (snirMin > snrThreshold) {
		if(packetOk(snirMin, frame->getBitLength() - (int)PHY_HEADER_LENGTH, payloadBitrate)) {
			result = new DeciderResult80211(true, payloadBitrate, snirMin);
		} else {
			EV << "Packet has BIT ERRORS! It is lost!\n";
			result = new DeciderResult80211(false, payloadBitrate, snirMin);
		}
	} else {
		EV << "Packet has ERRORS! It is lost!\n";
		result = new DeciderResult80211(false, payloadBitrate, snirMin);
	}

	delete snrMap;
	snrMap = 0;

	return result;
}

bool Decider80211p::packetOk(double snirMin, int lengthMPDU, double bitrate)
{
    double berHeader, berMPDU;

    //802.11g rates
	//PLCP Header 24bits, BPSK, r=1/2, 3Mbps
	berHeader=ber_bpsk(snirMin, BANDWIDTH , 3E+6, channelModel);
	berHeader=Pb(1, berHeader);

	switch ((int)bitrate) //added by Sorin Cocorada
	{

	case (int)(3E+6)://3Mbps, r=1/2, BPSK
		berMPDU=ber_bpsk(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(1, berMPDU);
		break;

	case (int)(4.5E+6)://4.5Mbps, r=3/4, BPSK
		berMPDU=ber_bpsk(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(3, berMPDU);
		break;

	case (int)(6E+6)://6Mbps, r=1/2, QPSK
		berMPDU=ber_qpsk(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(1, berMPDU);
		break;

	case (int)(9E+6)://9Mbps, r=3/4, QPSK
		berMPDU=ber_qpsk(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(3, berMPDU);
		break;

	case (int)(12E+6)://12Mbps, r=1/2, 16QAM
		berMPDU=ber_16qam(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(1, berMPDU);
		break;

	case (int)(18E+6)://18Mbps, r=3/4, 16QAM
		berMPDU=ber_16qam(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(3, berMPDU);
		break;

	case (int)(24E+6)://24Mbps, r=2/3, 64QAM
		berMPDU=ber_64qam(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(2, berMPDU);
		break;

	case (int)(27E+6)://27Mbps, r=3/4, 64QAM
		berMPDU=ber_64qam(snirMin, BANDWIDTH , bitrate, channelModel);
		berMPDU=Pb(3, berMPDU);
		break;

	default:
		berMPDU=0;
	}

    //probability of no bit error in the PLCP header
    double headerNoError = pow(1.0 - berHeader, 24);//PLCP Header 24bit(without SERVICE), 6Mbps


    //probability of no bit error in the MPDU
    double MpduNoError = pow(1.0 - berMPDU, lengthMPDU);
    EV << "berHeader: " << berHeader << " berMPDU: " <<berMPDU <<" length: "<<lengthMPDU<<" PER: "<<1-MpduNoError<<endl;
    double rand = dblrand();

    if (rand > headerNoError)
        return false; // error in header
    else if (dblrand() > MpduNoError)
        return false;  // error in MPDU
    else
        return true; // no error
}

simtime_t Decider80211p::processSignalEnd(AirFrame* frame)
{
	// here the Signal is finally processed

	DeciderResult* result = checkIfSignalOk(frame);

	if (result->isSignalCorrect())
	{
		EV << "packet was received correctly, it is now handed to upper layer...\n";
		// go on with processing this AirFrame, send it to the Mac-Layer
		phy->sendUp(frame, result);
	} else
	{
		EV << "packet was not received correctly, sending it as control message to upper layer\n";
		Mac80211Pkt* mac = static_cast<Mac80211Pkt*>(frame->decapsulate());
		mac->setName("ERROR");
		mac->setKind(BITERROR);
		phy->sendControlMsg(mac);
		delete result;
	}

	// we have processed this AirFrame and we prepare to receive the next one
	currentSignal.first = 0;

	//channel is idle now
	setChannelIdleStatus(true);

	return notAgain;
}
