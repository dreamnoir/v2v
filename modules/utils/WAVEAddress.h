#ifndef WAVEADDRESS_H
#define WAVEADDRESS_H

/**
 * @brief Define some addresses for the simple layers
 *
 * @ingroup baseUtils
 * @ingroup utils
 * @author Andreas K�pke
 */
enum {
    L3BROADCAST = -2,
    L2BROADCAST = -1,
};

enum
{
	SSH1 = 172,
	SSH2 = 174,
	SSH3 = 176,
	CCH = 178,
	SCH4 = 180,
	SCH5 = 182,
	SCH6 = 184,
};

enum
{
	THREEMBS = 3000000,
	SIXMBS = 6000000,
	NINEMBS = 9000000,
	TWENTYSEVENMPS = 27000000,
};

#endif
