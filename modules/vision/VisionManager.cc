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


#include "VisionManager.h"
#include "BaseWorldUtility.h"
#include "CCWSApplLayer.h"
#include "FindModule.h"
#include <cassert>

Define_Module(VisionManager);

void VisionManager::initialize(int stage)
{
	if (stage == 0)
	{
		maxDistance = par("maxDistance").doubleValue();
		visionCutoff = par("visionCutoff").doubleValue();
		debug = false;

		trackedVec.setName("vehicles-tracked");

		vehiclesTracked = 0;
	}
	else if (stage == 1)
	{
		if (debug) ev <<"initializing VisionManager\n";

		BaseWorldUtility* world = FindModule<BaseWorldUtility*>::findGlobalModule();

		assert(world != 0);

		playgroundSize = world->getPgs();
		useTorus = world->useTorus();

		maxDistSquared = maxDistance * maxDistance;

		//----initialize node grid-----
		//step 1 - calculate dimension of grid
		//one cell should have at least the size of maxDistance
		//but also should divide the playground in equal parts
		Coord dim((*playgroundSize) / maxDistance);
		gridDim = GridCoord(dim);

		//A grid smaller or equal to 3x3 whould mean that every cell has every other cell as direct
		//neighbor (if our playground is a torus, even if not the most of the cells are direct
		//neighbors of each other. So we reduce the grid size to 1x1.
		if((gridDim.x <= 3) && (gridDim.y <= 3) && (gridDim.z <= 3))
		{
			gridDim.x = 1;
			gridDim.y = 1;
			gridDim.z = 1;
		} else {
			gridDim.x = std::max(1, gridDim.x);
			gridDim.y = std::max(1, gridDim.y);
			gridDim.z = std::max(1, gridDim.z);
		}

		//step 2 - initialize the matrix which represents our grid
		VisionEntries entries;
		RowVector row;
		VisionMatrix matrix;

		for (int i = 0; i < gridDim.z; ++i) {
			row.push_back(entries);					//copy empty NicEntries to RowVector
		}
		for (int i = 0; i < gridDim.y; ++i) {	//fill the ColVector with copies of
			matrix.push_back(row);					//the RowVector.
		}
		for (int i = 0; i < gridDim.x; ++i) {	//fill the grid with copies of
			vehicleGrid.push_back(matrix);				//the matrix.
		}
		if (debug) ev << " using " << gridDim.x << "x" <<
							 gridDim.y << "x" <<
							 gridDim.z << " grid" << endl;

		//step 3 -	calculate the factor which maps the coordinate of a node
		//			to the grid cell

		if (gridDim.x == 1 &&												//if we use a 1x1 grid
			gridDim.y == 1 &&												//every coordinate is
			gridDim.z == 1) {									 			//mapped to (0,0, 0)
			findDistance = Coord(std::max(playgroundSize->getX(), maxDistance),
								 std::max(playgroundSize->getY(), maxDistance),
								 std::max(playgroundSize->getZ(), maxDistance));
		} else {
			findDistance = Coord(playgroundSize->getX() / gridDim.x,		//otherwise the factor is our
								 playgroundSize->getY() / gridDim.y,		//playground divided by the
								 playgroundSize->getZ() / gridDim.z);		//number of cells
		}

		//since the upper playground borders (at pg-size) are part of the
		//playground we have to assure that they are mapped to a valid
		//(the last) grid cell we do this by increasing the find distance
		//by a small value.
		//This also assures that findDistance is never zero.
		findDistance += Coord(EPSILON, EPSILON, EPSILON);

		//findDistance (equals cell size) has to be greater or equal maxInt-distance
		assert(findDistance.getX() >= maxDistance);
		assert(findDistance.getY() >= maxDistance);
		assert(world->use2D() || findDistance.getZ() >= maxDistance);

		//playGroundSize has to be part of the playGround
		assert(GridCoord(*playgroundSize, findDistance).x == gridDim.x - 1);
		assert(GridCoord(*playgroundSize, findDistance).y == gridDim.y - 1);
		assert(GridCoord(*playgroundSize, findDistance).z == gridDim.z - 1);
		if (debug) ev << "findDistance is " << findDistance.info() << endl;

		timer = new cMessage( "vision-timer", VISION_UPDATE);
		scheduleAt(simTime() + 0.100000001, timer);
	}
}

VisionManager::GridCoord VisionManager::getCellForCoordinate(const Coord& c) {
    return GridCoord(c, findDistance);
}

void VisionManager::handleMessage(cMessage* msg)
{
	switch(msg->getKind())
	{
		case VISION_UPDATE:

			ev << "updating vision..." << simTime() << endl;

			visionUpdate();


			if (vehiclesTracked != 0)
			{
				timer = new cMessage( "vision-timer", VISION_UPDATE);
				scheduleAt(simTime() + 0.1, timer);
			}
			break;
	}

	delete msg;
}

void VisionManager::visionUpdate()
{

	VisionEntries::iterator it = vehicles.begin();
	while (it != vehicles.end())
	{
		if ((*it).second != 0)
		{
			GridCoord cell = getCellForCoordinate(((*it).second)->pos);
			checkGrid(cell, it, (*it).first);
		}
		it++;
	}
}

void VisionManager::updateConnections(int nicID, const Coord* oldPos, const Coord* newPos)
{
	GridCoord oldCell = getCellForCoordinate(*oldPos);
    GridCoord newCell = getCellForCoordinate(*newPos);

	//checkGrid(oldCell, newCell, nicID );

    // find nic at old position
    VisionEntries& oldCellEntries = getCellEntries(oldCell);
    VisionEntries::iterator it = oldCellEntries.find(nicID);
    VisionEntry *nic = it->second;

    // move nic to a new position in matrix
    if(oldCell != newCell) {
    	oldCellEntries.erase(it);
    	getCellEntries(newCell)[nicID] = nic;
    }
}

VisionManager::VisionEntries& VisionManager::getCellEntries(VisionManager::GridCoord& cell) {
    return vehicleGrid[cell.x][cell.y][cell.z];
}

void VisionManager::registerVehicleExt(int nicID)
{
	VisionEntry* VisionEntry = vehicles[nicID];

	GridCoord cell = getCellForCoordinate(VisionEntry->pos);

	if (debug) ev <<" registering (ext) nic at loc " << cell.info() << std::endl;

	// add to matrix
	VisionEntries& cellEntries = getCellEntries(cell);
    cellEntries[nicID] = VisionEntry;
}

void VisionManager::checkGrid(VisionManager::GridCoord& cell,
									VisionEntries::iterator& it,
                                      int id)

{

    // structure to find union of grid squares
    CoordSet gridUnion(74);

    VisionEntry* vehicle = vehicles[id];

	if((gridDim.x == 1) && (gridDim.y == 1) && (gridDim.z == 1)) {
		gridUnion.add(cell);
    } else {
		fillUnionWithNeighbors(gridUnion, cell);
    }

	if (vehicle->isVisionOn())
	{
		// clear out vehicles in range
		vehicle->withinRange.clear();

		// update all vehicles in nearby grids to check for range
	    GridCoord* c = gridUnion.next();
	    while(c != 0) {
			updateVehicleVision(getCellEntries(*c), vehicle);
			c = gridUnion.next();
	    }

	    // prune list of maybe visible vehicles
	    vehicle->pruneVisible(visionCutoff);

	    if (debug) ev << "visible by " << id << " are " << vehicle->visible << " of " << vehicle->possible << " vehicles." << endl;
	}
}

int VisionManager::wrapIfTorus(int value, int max) {
	if(value < 0) {
		if(useTorus) {
			return max + value;
		} else {
			return -1;
		}
	} else if(value >= max) {
		if(useTorus) {
			return value - max;
		} else {
			return -1;
		}
	} else {
		return value;
	}
}

void VisionManager::fillUnionWithNeighbors(CoordSet& gridUnion, GridCoord cell) {
	for(int iz = (int)cell.z - 1; iz <= (int)cell.z + 1; iz++) {
		if(iz != cell.z && cell.use2D) {
			continue;
		}
		int cz = wrapIfTorus(iz, gridDim.z);
		if(cz == -1) {
			continue;
		}
		for(int ix = (int)cell.x - 1; ix <= (int)cell.x + 1; ix++) {
			int cx = wrapIfTorus(ix, gridDim.x);
			if(cx == -1) {
				continue;
			}
			for(int iy = (int)cell.y - 1; iy <= (int)cell.y + 1; iy++) {
				int cy = wrapIfTorus(iy, gridDim.y);
				if(cy != -1) {
					if(cell.use2D) {
						gridUnion.add(GridCoord(cx, cy));
					} else {
						gridUnion.add(GridCoord(cx, cy, cz));
					}
				}
			}
		}
	}
}

void VisionManager::updateVehicleVision(VisionEntries& nmap, VisionEntry* nic)
{
    int id = nic->vehicleId;

    for(VisionEntries::iterator i = nmap.begin(); i != nmap.end(); ++i)
    {
		VisionEntry* nic_i = i->second;

        // no recursive connections
        if ( nic_i->vehicleId == id ) continue;

		double distance;

        if(useTorus)
        {
        	distance = nic->pos.sqrTorusDist(nic_i->pos, playgroundSize);
        } else {
        	distance = nic->pos.sqrdist(nic_i->pos);
        }

        bool inRange = (distance <= maxDistSquared);


        if (inRange)
        {
            double angle = nic->getAngleTo(nic_i);

			bool vis = false;

			distance = nic->getDistanceTo(nic_i);

			double distances[] = {150, 20, 15, 15, 15};

            if (angle >= 345 || angle <= 15)
            {
            	// Zone A
            	if (distance < distances[0])
            		vis = true;
            }
            else if ((angle > 15 && angle <45) || (angle > 315 && angle < 345))
            {
            	//Zone B
            	if (distance < distances[1])
            		vis = true;
            }
            else if (angle >= 45 && angle <= 135)
            {
            	//Zone C
            	if (distance < distances[2])
            		vis = true;
            }
            else if (angle >135 && angle < 225)
            {
            	//Zone D
            	if (distance < distances[3])
            		vis = true;
            }
            else
            {
            	if (distance < distances[4])
            		vis = true;
            }

            if (vis)
            {

            	MinMax angles = nic->getMinMaxAngles(nic_i);
            	if (debug) ev << "Vehicle ID(" << nic->vehicleId << " can see ID" << nic_i->vehicleId << " at distance " << distance << " and at angle " << angle << " with angles to corners (" << angles.min << ", " << angles.max << ")" << endl;

            	// create new visible vehicle holder
            	VisibleVehicle v = {nic_i->vehicleId, nic_i, distance, true, angles};

            	// find correct position based on distance and insert into list
            	VehicleList::iterator ci = nic->withinRange.begin();
            	while (ci != nic->withinRange.end() && (*ci).distance < distance)
            		ci++;
            	nic->withinRange.insert(ci, v);

            }
        }
    }
}

bool VisionManager::registerVehicle(cModule* nic, const Coord* vehiclePos, const Coord* vehicleAngle, double length, double width, bool vision)
{
	assert(nic != 0);

	int nicID = nic->getParentModule()->getIndex();

	// create new VisionEntry
	VisionEntry *visionEntry;

	ev << "vision added vehicle with length=" << length << " and width=" << width << endl;

	visionEntry = new VisionEntry(length, width, vision);

	// fill VisionEntry
	visionEntry->appPtr = nic;
	visionEntry->vehicleId = nicID;
	visionEntry->pos =  vehiclePos;
	visionEntry->angle = vehicleAngle;

	// add to map
	vehicles[nicID] = visionEntry;

	registerVehicleExt(nicID);

	updateConnections(nicID, vehiclePos, vehiclePos);

	trackedVec.record(++vehiclesTracked);

	return true;
}

bool VisionManager::unregisterVehicle(cModule* nicModule)
{
	assert(nicModule != 0);

	// find VisionEntry
	int nicID = nicModule->getParentModule()->getIndex();
	if (debug) ev << " unregistering nic #" << nicID << endl;

	//we assume that the module was previously registered with this CM
	//TODO: maybe change this to an omnet-error instead of an assertion
	assert(vehicles.find(nicID) != vehicles.end());
	VisionEntry* visionEntry = vehicles[nicID];

	// get all affected grid squares
	CoordSet gridUnion(74);
	GridCoord cell = getCellForCoordinate(visionEntry->pos);
	if((gridDim.x == 1) && (gridDim.y == 1) && (gridDim.z == 1)) {
		gridUnion.add(cell);
	} else {
		fillUnionWithNeighbors(gridUnion, cell);
	}

	// erase from grid
	VisionEntries& cellEntries = getCellEntries(cell);
	cellEntries.erase(nicID);

	// erase from list of known nics
	vehicles[nicID] = (VisionEntry*)0;

	// delete the object
	delete visionEntry;

	trackedVec.record(--vehiclesTracked);

	return true;
}

void VisionManager::updateVehiclePos(int nicID, const Coord* newPos, const Coord* newAngle)
{
	VisionEntry* VisionEntry = vehicles[nicID];
	if(VisionEntry == 0)
		error("No nic with this ID is registered with this ConnectionManager.");

    Coord oldPos = VisionEntry->pos;
    VisionEntry->pos = newPos;
    VisionEntry->angle = newAngle;

	updateConnections(nicID, &oldPos, newPos);
}

int VisionManager::visible(int vehicleID)
{
	return vehicles[vehicleID]->visible;
}

int VisionManager::maybeVisible(int vehicleID)
{
	return vehicles[vehicleID]->possible;
}

Coord VisionManager::getVehiclePos(int vehicleID)
{
	return ((CCWSApplLayer*)vehicles[vehicleID]->appPtr)->getCurrentPos();
}

double VisionManager::getWidth(int vehicleID)
{
	return vehicles[vehicleID]->getWidth();
}
double VisionManager::getLength(int vehicleID)
{
	return vehicles[vehicleID]->getLength();
}

bool VisionManager::vehicleExists(int vehicleID)
{
	return (vehicles[vehicleID] != 0);
}

VehicleList VisionManager::getVisible(int vehicleID)
{
	return vehicles[vehicleID]->withinRange;
}

VisionManager::~VisionManager()
{
	for (VisionEntries::iterator ne = vehicles.begin(); ne != vehicles.end(); ne++)
	{
		delete ne->second;
	}
}
