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

package v2v.modules.application;

import org.mixim.base.modules.IBaseApplLayer;

simple CCWSApplLayer like IBaseApplLayer
{
    parameters:
    	double autoRetransmitTime @unit("s") = default (1s); // time before auto retransmit in threshold mode 
    	double delay @unit("s") = default(0.5s); // delay interval between updates or threshold checks
    	double thresholdSize @unit("m") = default(1m); // error threshold size for threshold mode
    	int maxVehicles; // max vehicles in simulation
    	double nveTimeout @unit("s") = default(3s); // how long before removing NVEs
    	bool thresholdMode = default(false); // timed update or error threshold updates
    	bool debug; // debug switch			
    	int headerLength @unit("bit"); // length of the application message header (in bits)
    	
    	bool retransmit = default(true); // retransmit second time in threshold mode
    	double retransmitTime @unit("s") = default(0.05s); // time before second retransmit
    	   
    gates:
    	input lowerGateIn; // from network layer
        output lowerGateOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer
}