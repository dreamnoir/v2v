//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package v2v.modules.mac;

import org.mixim.base.modules.BaseMacLayer;

//
// Implementation of the 802.11b MAC protocol. This module is intended
// to be used in combination with the Decider80211 as decider for the 
// physical layer.
//
// This module supports ad-hoc mode only (does not generate or handle
// management frames), and does not support fragmentation, RTS threshold,
// duplication detection and recovery.
//
simple Mac80211p extends BaseMacLayer
{
    parameters:
        @class(Mac80211p);
        
        int queueLength;
        // Number of bits in a packet before RTS/CTS is used. 
        // Is currently not used.
        int rtsCtsThreshold = default(-1); 

        double bitrate @unit(bps);

    	// auto bit rate selection support: specify borders at which
        // to change modulation.	
    	bool autoBitrate;         // use auto bit rate adaptation

    	// keep information on neighborhood
    	int neighborhoodCacheSize;	
    	// consider information in cache outdate if older than (in seconds)
    	double neighborhoodCacheMaxAge @unit(s); 
    	//the power to transmit packets with [mW]
    	double txPowerMax @unit(mW);
    	double txPowerMin @unit(mW);
    	
    	bool trackCCA = default(false);
    	
    	headerLength = default(272bit);
}

