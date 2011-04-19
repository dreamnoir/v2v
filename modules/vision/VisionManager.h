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

#ifndef VISIONMANAGER_H_
#define VISIONMANAGER_H_

#include "VisionEntry.h"

class VisionManager : public cSimpleModule
{
private:
	/**
	 * @brief Represents a position inside a grid.
	 *
	 * Internal helper class of BaseConnectionManager.
	 * This class provides some converting functions from a Coord
	 * to a GridCoord.
	 */
	class GridCoord
	{
	public:
		/** @brief Constant for undefined coordinates.*/
		static const int UNDEFINED = 0;
		/** @name Coordinates in the grid.*/
		/*@{*/
		int x;
		int y;
		int z;
		/*@}*/
		/** @brief Use 2D or 3D coordinates?*/
		bool use2D;

	public:
		/**
		 * @brief Initialize this GridCoord with the origin.
		 * Creates a 3-dimensional coord.
		 */
		GridCoord()
			:x(0), y(0), z(0), use2D(false) {};

		/**
		 * @brief Initialize a 2-dimensional GridCoord with x and y.
		 */
		GridCoord(int x, int y)
			:x(x), y(y), z(UNDEFINED), use2D(true) {};

		/**
		 * @brief Initialize a 3-dimensional GridCoord with x, y and z.
		 */
		GridCoord(int x, int y, int z)
			:x(x), y(y), z(z), use2D(false) {};

		/**
		 * @brief Simple copy-constructor.
		 */
		GridCoord(const GridCoord& o) {
			x = o.x;
			y = o.y;
			z = o.z;
			use2D = o.use2D;
        }

		/**
		 * @brief Creates a GridCoord from a given Coord by dividing the
		 * x,y and z-values by "gridCellWidth".
		 * The dimension of the GridCoord depends on the Coord.
		 */
        GridCoord(const Coord& c, const Coord& gridCellSize = Coord(1.0,1.0,1.0)) {
            x = static_cast<int>(c.getX() / gridCellSize.getX());
            y = static_cast<int>(c.getY() / gridCellSize.getY());
            z = static_cast<int>(c.getZ() / gridCellSize.getZ());
            use2D = c.is2D();
        }

        /** @brief Output string for this coordinate.*/
        std::string info() const {
			std::stringstream os;
			if (use2D) {
				os << "(" << x << "," << y << ")";
			} else {
				os << "(" << x << "," << y << "," << z << ")";
			}
			return os.str();
		}

        /** @brief Comparison operator for coordinates.*/
		friend bool operator==(const GridCoord& a, const GridCoord& b) {
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}

		/** @brief Comparison operator for coordinates.*/
		friend bool operator!=(const GridCoord& a, const GridCoord& b) {
			return !(a==b);
		}
	};

	/**
	 * @brief Represents an minimalistic (hash)set of GridCoords.
	 *
	 * It is a workaround because c++ doesn't come with an hash set.
	 */
	class CoordSet {
	protected:
		/** @brief Holds the hash table.*/
		std::vector<GridCoord*> data;
		/** @brief maximum size of the hash table.*/
		unsigned maxSize;
		/** @brief Current number of entries in the hash table.*/
		unsigned size;
		/** @brief Holds the current element when iterating over this table.*/
		unsigned current;

	protected:

		/**
		 * @brief Tries to insert a GridCoord at the specified position.
		 *
		 * If the same Coord already exists there nothing happens.
		 * If an other Coord already exists there calculate
		 * a new Position to insert end recursively call this Method again.
		 * If the spot is empty the Coord is inserted.
		 */
		void insert(const GridCoord& c, unsigned pos) {
			if(data[pos] == 0) {
				data[pos] = new GridCoord(c);
				size++;
			} else {
				if(*data[pos] != c) {
					insert(c, (pos + 2) % maxSize);
				}
			}
		}

	public:
		/**
		 * @brief Initializes the set (hashtable) with the a specified size.
		 */
		CoordSet(unsigned sz)
			:maxSize(sz), size(0), current(0)
		{
			data.resize(maxSize);
		}

		/**
		 * @brief Delete every created GridCoord
		 */
		~CoordSet() {
			for(unsigned i = 0; i < maxSize; i++) {
				if(data[i] != 0) {
					delete data[i];
				}
			}
		}

		/**
		 * @brief Adds a GridCoord to the set.
		 * If a GridCoord with the same value already exists in the set
		 * nothing happens.
		 */
		void add(const GridCoord& c) {
			unsigned hash = (c.x * 10000 + c.y * 100 + c.z) % maxSize;
			insert(c, hash);
		}

		/**
		 * @brief Returns the next GridCoord in the set.
		 * You can iterate through the set only one time with this function!
		 */
		GridCoord* next() {
			for(;current < maxSize; current++) {
				if(data[current] != 0) {
					return data[current++];
				}
			}
			return 0;
		}

		/**
		 * @brief Returns the number of GridCoords currently saved in this set.
		 */
		unsigned getSize() { return size; }

		/**
		 * @brief Returns the maximum number of elements which can be stored inside
		 * this set.
		 * To prevent collisions the set should never be more than 75% filled.
		 */
		unsigned getmaxSize() { return maxSize; }
	};

protected:
	/** @brief Type for map from nic-module id to nic-module pointer.*/
	typedef std::map<int, VisionEntry*> VisionEntries;

	/** @brief Map from nic-module ids to nic-module pointers.*/
	VisionEntries nics;

	/** @brief Stores the size of the playground.*/
	const Coord* playgroundSize;

	/** @brief the biggest vision distance for any camera*/
	double maxDistance;

	/** @brief Square of maxInterferenceDistance cache a value that
	 * is often used */
	double maxDistSquared;

	/** @brief Stores the useTorus flag of the WorldUtility */
	bool useTorus;

	/** @brief Type for 1-dimensional array of VisionEntries.*/
	typedef std::vector<VisionEntries> RowVector;
	/** @brief Type for 2-dimensional array of VisionEntries.*/
	typedef std::vector<RowVector> VisionMatrix;
	/** @brief Type for 3-dimensional array of VisionEntries.*/
    typedef std::vector<VisionMatrix> VisionCube;

	/**
	 * @brief Register of all nics
     *
     * This matrix keeps all nics according to their position.  It
     * allows to restrict the position update to a subset of all nics.
     */
    VisionCube nicGrid;

    /**
     * @brief Distance that helps to find a node under a certain
     * position.
     *
     * Can be larger then @see maxInterferenceDistance to
     * allow nodes to be placed into the same square if the playground
     * is too small for the grid speedup to work.
	 */
    Coord findDistance;

    /** @brief The size of the grid */
    GridCoord gridDim;

    bool debug;

    // percentage of vehicle required to be visible
    double visionCutoff;

private:
	/** @brief Manages the connections of a registered nic. */
    void updateNicConnections(VisionEntries& nmap, VisionEntry* nic);

    /**
     * @brief Check connections of a nic in the grid
     */
    void checkGrid(GridCoord& oldCell,
                   GridCoord& newCell,
                   int id);

    /**
     * @brief Calculates the corresponding cell of a coordinate.
     */
    GridCoord getCellForCoordinate(const Coord& c);

    /**
     * @brief Returns the VisionEntries of the cell with specified
     * coordinate.
     */
    VisionEntries& getCellEntries(GridCoord& cell);

	/**
	 * If the value is outside of its bounds (zero and max) this function
	 * returns -1 if useTorus is false and the wrapped value if useTorus is true.
	 * Otherwise its just returns the value unchanged.
	 */
    int wrapIfTorus(int value, int max);

	/**
	 * @brief Adds every direct Neighbor of a GridCoord to a union of coords.
	 */
    void fillUnionWithNeighbors(CoordSet& gridUnion, GridCoord cell);
protected:

	/**
	 * @brief Called by "registerNic()" after the nic has been
	 * registered. That means that the VisionEntry for the nic has already been
	 * created and added to nics map.
	 *
	 * You better know what you are doing if you want to override this
	 * method. Most time you won't need to.
	 *
	 * See ConnectionManager::registerNicExt() for an example.
	 *
	 * @param nicID - the id of the VisionEntry
	 */
	virtual void registerNicExt(int nicID);

	/**
	 * @brief Updates the connections of the nic with "nicID".
	 *
	 * This method is called by "updateNicPos()" after the
	 * new Position is stored in the corresponding nic.
	 *
	 * Most time you won't need to override this method.
	 *
	 * @param nicID the id of the VisionEntry
	 * @param oldPos the old position of the nic
	 * @param newPos the new position of the nic
	 */
	virtual void updateConnections(int nicID, const Coord* oldPos, const Coord* newPos);

public:

	virtual ~VisionManager();

	void initialize(int stage);

	int visible(int vehicleID);
	int maybeVisible(int vehicleID);

	/** @brief Needs two initialization stages.*/
	virtual int numInitStages() const {
		return 2;
	}

	/**
	 * @brief Registers a nic to have its connections managed by ConnectionManager.
	 *
	 * If you want to do your own stuff at the registration of a nic see
	 * "registerNicExt()".
	 */
	bool registerNic(cModule* nic, const Coord* vehiclePos, const Coord* vehicleAngle);

	/**
	 * @brief Unregisters a NIC such that its connections aren't managed by the CM
	 * anymore.
	 *
	 * NOTE: This method asserts that the passed NIC module was previously registered
	 * with this ConnectionManager!
	 *
	 * This method should be used for dynamic networks were hosts can actually disappear.
	 *
	 * @param nic the NIC module to be unregistered
	 * @return returns true if the NIC was unregistered successfully
	 */
	bool unregisterNic(cModule* nic);

	/** @brief Updates the position information of a registered nic.*/
	void updateNicPos(int nicID, const Coord* newPos, const Coord* newAngle);

};
#endif /* VISIONMANAGER_H_ */
