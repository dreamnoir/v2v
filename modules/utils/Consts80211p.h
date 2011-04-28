
#ifndef CONSTANTS_802_11
#define CONSTANTS_802_11

/** @brief Bit rates for 802.11b */
const double BITRATES_80211[] = {
    3000000,
    6000000,
    9000000,
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

/** @brief Length of PLCP header and Preamble */
const double PHY_HEADER_LENGTH=192;
/** @brief PLCP header */
const double HEADER_WITHOUT_PREAMBLE=48;
/** @brief Bitrate with which the header is send */
const double BITRATE_HEADER=1E+6;
/** @brief 3dB filter bandwidth -- channels overlap! */
const double BANDWIDTH=10E+6;

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

