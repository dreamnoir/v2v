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

simple TimedApplLayer like IBaseApplLayer
{
    parameters:
    	double delay @unit("s") = default(0.5s);
    	double thresholdSize @unit("m") = default(1m);
    	int maxVehicles;
    	int nveTimeout @unit("s") = default(3s);
    	bool thresholdMode = default(false);
    	bool debug; // debug switch
    	int headerLength @unit("bit"); // length of the application message header (in bits)
    	
    	bool retransmit = default(true);
    	double retransmitTime @unit("s") = default(0.05s);
    	   
    gates:
    	input lowerGateIn; // from network layer
        output lowerGateOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer
}