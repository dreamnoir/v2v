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
	CCH = 1,
	SCH1,
	SCH2,
	SCH3,
};

enum
{
	THREEMBS = 1,
	SIXMBS,
	NINEMBS,
	TWENTYSEVENMPS,
};

#endif
