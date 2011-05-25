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

#include "VisionMap.h"

VisionMap::VisionMap(double cutoff, bool debug)
{
	this->cutoff = cutoff;
	this->debug = debug;
}

void VisionMap::addAngle(MinMax angles)
{
	if (debug) ev << "addToList()" << endl;

	// list of angles current angles intersect with
	std::list<MinMaxList::iterator> intersections;

	MinMaxList::iterator li = list.begin();
	while (li != list.end() && (*li)<angles)
	{
		if (debug) ev << "checking angles (" << angles.min << "," << angles.max << ") against (" << (*li).min << "," << (*li).max  << ")" << endl;

		// check if there is any intersection
		if (angles.intersection(*li))
		{
			if (debug) ev << "intersection" << endl;
			intersections.push_back(li);
		}
		li++;
	}

	// no intersections
	if (intersections.size() == 0)
	{
		if (debug) ev << "just insert" << endl;
		list.insert(li, angles);
	}
	else
	{
		// find minimum and maximum coverage
		double min = angles.min<(intersections.front())->min ? angles.min : (intersections.front())->min;
		double max = angles.max>(intersections.back())->max ? angles.max : (intersections.back())->max;

		// erase intersecting angles
		if (intersections.size() == 1)
			list.erase(intersections.front());
		else
			list.erase(intersections.front(), (++intersections.back()));

		// add new min and max to list
		MinMax add = {min, max};
		list.insert(li, add);
	}

	if (debug) ev << "addToList() done now with " << list.size() << " items." << endl;
}

double VisionMap::visibleRange(MinMax angles)
{
	if (debug) ev << "reallyVisible()" << endl;

	double minToMax = angles.minToMax();

	MinMaxList::iterator li = list.begin();
	while (li != list.end() && (*li)<angles)
	{
		if (debug) ev << "checking angles (" << angles.min << "," << angles.max << ") against (" << (*li).min << "," << (*li).max  << ")" << endl;

		if (angles.intersection(*li))
		{
			if ((*li).bothInside(angles))
			{
				minToMax = 0;
				if (debug) ev << "both inside minToMax = " << minToMax << endl;
			}
			else if (angles.bothInside(*li))
			{
				minToMax -= (*li).minToMax();
				if (debug) ev << "both inside backwards minToMax = " << minToMax << endl;
			}
			else if ((*li).minInside(angles))
			{
				minToMax -= (*li).max - angles.min;
				if (debug) ev << "min inside minToMax = " << minToMax << endl;
			}
			else if ((*li).maxInside(angles))
			{
				minToMax -= angles.max - (*li).min;
				if (debug) ev << "max inside minToMax = " << minToMax << endl;
			}
		}
		li++;
	}

	return minToMax;
}

void VisionMap::add(MinMax angles)
{
	// check if angle range is crosses over 360% and treat appropriately
	if (angles.min > angles.max)
	{
		MinMax low = {0, angles.max};
		MinMax high = {angles.min, 360.0};
		addAngle(low);
		addAngle(high);
	}
	else
	{
		addAngle(angles);
	}
}

bool VisionMap::visible(MinMax angles)
{
	double minToMax;

	// check if angle range crosses over 360 degrees and treat appropriately
	if (angles.min > angles.max)
	{
		MinMax low = {0, angles.max};
		MinMax high = {angles.min, 360.0};
		minToMax = visibleRange(low) + visibleRange(high);
	}
	else
	{
		minToMax = visibleRange(angles);
	}

	// check how much of total is visible as percent compared to cutoff to judge visiblity
	if (minToMax == 0)
	{
		if (debug) ev << "complete intersection!" << endl;
		return 0;
	}
	else if (minToMax / angles.minToMax() < cutoff)
	{
		if (debug) ev << "over " << cutoff*100 << "% intersection!" << endl;
		return false;
	}
	else
	{
		if (debug) ev << "less than 50% intersection!" << endl;
		return true;
	}
}
