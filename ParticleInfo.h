#ifndef PARTICLEINFO_H
#define PARTICLEINFO_H

#include <vector>
#include <set>
#include <algorithm>
#include "Coordinate.h"
#include "TaggedVector.h"
#include "Particle.h"
#include "MiscStructs.h"

class ParticleInfo
{
	private:
		int numParticles;
		std::vector <struct Coordinate> v_pos;
		std::vector <struct Coordinate> v_force;
		std::vector <double> v_mass;
        
		std::vector <int> v_bins;
		std::vector<std::vector<struct HalfBond>> v_bondTags;
        std::vector<std::vector<struct Angle>> v_angleTags;
		std::vector <struct Bond> v_bonds;
        std::vector <struct Angle> v_angles;
        std::vector <std::vector <int>> v_bondNeighborTags;
        std::vector <int> v_beadToFilament;
        std::vector <std::vector<struct Coordinate>> v_diagonalStress;
        std::vector <int> v_viscosityType;
        
        std::vector <std::vector<int>> v_neighborTags;
        
        TaggedVector tagVector;
        TaggedVector bondTagVector;
        TaggedVector angleTagVector;
        
	public:
		ParticleInfo();
		
		int addParticle(Particle &p, int numStressMeasurements);
		void removeParticle(int particleKey);
		
		struct Coordinate getPos(int particleKey);
		struct Coordinate getPosByIndex(int particleIndex);
		void setPos(int particleTag, struct Coordinate c);
		void setPosByIndex(int particleIndex, struct Coordinate c);
		
		struct Coordinate getForce(int particleTag);
		struct Coordinate getForceByIndex(int particleIndex);
		void setForce(int particleTag, struct Coordinate c);
		void setForceByIndex(int particleIndex, struct Coordinate c);
		void addForce(int particleTag, struct Coordinate c);
		void addForceByIndex(int particleIndex, struct Coordinate c);
		std::vector <Coordinate> getAllPosByIndex();
		
        std::vector <struct Angle> getAngleTagsByIndex(int particleIndex);
        std::vector <struct Angle> getAngleTags(int particleTag);
        int addAngle(struct Angle newAngle);
        struct Angle getAngle(int aTag);
        struct Angle getAngleByIndex(int aIndex);
        void removeAngle(int aTag);
        void removeAngleByIndex(int aIndex);
        
        std::vector<int> & getBondTagsOldestToYoungest();
        int getBondTagsOldestToYoungestFront();
        
		//std::vector <int> getTagList();
		int getTagAtIndex(int pos);
		int getIndexOfTag(int tag);
		
		void setBinByIndex(int particleIndex, int bin);
		int getBinByIndex(int particleIndex);
		
		std::vector <struct HalfBond> getBondTagsByIndex(int particleIndex);
		std::vector <struct HalfBond> getBondTags(int particleTag);
        bool checkBondExists(struct Bond b);
		
		int getNumParticles();
        
        int getCurrMaxTag();
        void setTagAtPos(int pos, int newTag);
        
        bool checkTagExists(int tag);
		
		struct Bond getBond(int bTag);
        struct Bond getBondByIndex(int bIndex);
		int addBond(struct Bond newBond);
		int getNumBonds();
        int getNumAngles();
		void removeBond(int bTag);
        void removeBondByIndex(int bIndex);
        int getBondTagAtPos(int bPos);
        int getBondPosOfTag(int bTag);
        void setBondType(int bTag, int newType);
        
        int getBeadToFilament(int particleTag);
        int getBeadToFilamentByIndex(int particleIndex);
        void setBeadToFilament(int particleTag, int newFilamentIndex);
        void setBeadToFilamentByIndex(int particleIndex, int newFilamentIndex);
        
        std::vector <int> getNeighbors(int particleTag);
        void setNeighbors(int particleTag, std::vector <int> newNeighbors);
        void setNeighborsByIndex(int particleIndex, std::vector <int> newNeighbors);
        void addNeighbor(int particleTag, int newNeighbor);
        
        std::vector <int> & getBondNeighborTagsByIndex(int bondIndex);
        std::vector <int> & getBondNeighborTags(int bondTag);
        void setBondNeighborTags(int bondTag, std::vector <int> newNeighbors);
        void setBondNeighborTagsByIndex(int bondIndex, std::vector <int> newNeighbors);
        void resetBondNeighborTagsByIndex(int bondIndex, std::vector <int> possibleNeighborBondTypes);
        
        void resetDiagonalStress();
        struct Coordinate getStress(int particleTag, int stressIndex);
        struct Coordinate getStressByIndex(int particleIndex, int stressIndex);
        void setStress(int particleTag, struct Coordinate newStress, int stressIndex);
        void setStressByIndex(int particleIndex, struct Coordinate newStress, int stressIndex);
        void addStress(int particleTag, struct Coordinate newStress, int stressIndex);
        void addStressByIndex(int particleIndex, struct Coordinate newStress, int stressIndex);
        
        int getViscosityTypeByIndex(int particleIndex);
        void setViscosityTypeByIndex(int particleIndex, int newViscosityType);
        
        int findBondTag(int aTag, int bTag);
        
        void appendBondNeighborTags(int bondTag, std::vector <int> additionalNeighbors);
        void appendBondNeighborTagsByIndex(int bondIndex, std::vector <int> additionalNeighbors);
        
        void appendBondNeighborTag(int bondTag, int additionalNeighbors);
        void appendBondNeighborTagByIndex(int bondIndex, int additionalNeighbors);
};

#endif