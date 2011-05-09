
#ifndef CONSTANTS_802_11
#define CONSTANTS_802_11

/** @brief Bit rates for 802.11b */
const double BITRATES_80211[] = {
    3000000,
    4500000,
    6000000,
    9000000,
    12000000,
    18000000,
    24000000,
    27000000
};

/** @brief Center frequencies for 802.11b */
const double CENTER_FREQUENCIES[] = {
	-1, 		//channel 0 does not exist
    5.86e9,	// 1
    5.87e9,	// 2
    5.88e9,	// 3
    5.89e9,	// 4
    5.9e9,	// 5
    5.91e9,	// 6
    5.92e9,	// 7
};


/** @brief duration of the PHY header
 *
 * If the radio was switched to
 * late, a partially received header is still ok, the total received
 * packet duration can be shorter by this amount compared to the send
 * packet.
 */
const double RED_PHY_HEADER_DURATION = 0.000020;
/** @brief Preambule transmision delay */
const double PLCP_PREAMBLE_DELAY = 20E-6;
/** @brief Length of PLCP deader without SERVICE and Tail */
const int PLCP_HEADER_LENGTH = 4 + 1 + 12 + 1 + 6;
/** @brief SIGNAL delay */
const double PLCP_SIGNAL_DELAY = 4E-6;
/** @brief SYMBOL delay */
const double T_SYM = 4E-6;

// frame lengths in bits
// XXX this is duplicate, it's already in Ieee80211Frame.msg

/** @brief BasicBitrate with which the header is send */
const double HEADER_BITRATE=3E+6;
const int PHY_HEADER_LENGTH_A = 4 + 1 + 12 + 1 + 6 + 16 + 6;
const int PHY_HEADER_LENGTH_B = 192;
//const double PHY_HEADER_LENGTH_G = 26E-6;
const int HEADER_WITHOUT_PREAMBLE = 48;
const double BITRATE_HEADER = 3E+6;
const double BANDWIDTH = 1E+6;

// added all above from INETMANET

/** @brief Length of PLCP header and Preamble */
const double PHY_HEADER_LENGTH=192;

const int MAC_GENERATOR = 5;

/** @brief standard frame lengths (in bit)
    @{ */
const double LENGTH_RTS = 160;
const double LENGTH_CTS = 112;
const double LENGTH_ACK = 112;
/** @} */
const int MAC80211_HEADER_LENGTH = 272;

//time slot ST, short interframe space SIFS, distributed interframe
//space DIFS, and extended interframe space EIFS
/** @brief Standard time values
    @{ */
/** @brief Slot time  */
const const_simtime_t ST = 13E-6;
/** @brief Short interframe space
 *  between DATA and ACK
 */
const const_simtime_t SIFS = 32E-6;
/** @brief Distributed interframe space
 * medium must be clear for this time in DCF
 */
const const_simtime_t DIFS = 6*ST + SIFS;
/** @brief extended interframe space
 *  whenever a corrupted frame is received, defer actions for this time
 */
const const_simtime_t EIFS = SIFS + DIFS +  (PHY_HEADER_LENGTH + LENGTH_ACK)/BITRATE_HEADER;

/** @brief We have to stop transmitting somewhen.
 *
 * This is the default value from the standard, it should be configurable
 */
const unsigned LONG_RETRY_LIMIT = 4;

/** @brief Imcrement longRetryCounter when this limit is met
 *
 * This is the default value from the standard
 */
const unsigned SHORT_RETRY_LIMIT = 7;

/**
 * Minimum size (initial size) of contention window, assuming DS PHY
 */
const unsigned CW_MIN = 7;

/** Maximum size of contention window, assuming DS PHY */
const unsigned CW_MAX = 225;

#endif
