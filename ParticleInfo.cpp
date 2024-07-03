#include <iostream>
#include "ParticleInfo.h"

using namespace std;

ParticleInfo::ParticleInfo()
{
	numParticles = 0;
}

int ParticleInfo::addParticle(Particle &p, int numStressMeasurements)
{
	struct Coordinate c = {p.getPosition(0), p.getPosition(1), p.getPosition(2)};
	v_pos.push_back(c);
	c = {p.getForce(0), p.getForce(1), p.getForce(2)};
	v_force.push_back(c);
	v_mass.push_back(p.getMass());
	v_bins.push_back(-1);
    v_beadToFilament.push_back(p.getType());
	
	std::vector <struct HalfBond> halfBonds;
	v_bondTags.push_back(halfBonds);
    
    std::vector <struct Angle> newAngleList;
    v_angleTags.push_back(newAngleList);
    
    std::vector <int> newNeighbors;
    v_neighborTags.push_back(newNeighbors);
    
    struct Coordinate zeroStress = {0.0, 0.0, 0.0};
    std::vector <struct Coordinate> tempVec;
    
    v_viscosityType.push_back(0);
    
    for(int i = 0; i < numStressMeasurements; i++)
    {
        tempVec.push_back(zeroStress);
    }
    
    v_diagonalStress.push_back(tempVec);
	
    numParticles++;
	
    int newTag = tagVector.add();
    
	return newTag;
}

void ParticleInfo::removeParticle(int particleTag)
{
    int tempIndex = tagVector.getIndexOfTag(particleTag);
    
    if(tempIndex >= 0 && tempIndex < v_pos.size())
	{	
		numParticles--;

        // remove bonds associated with this particle
        std::vector <struct HalfBond> tempBondList = getBondTagsByIndex(tempIndex);
        for(int b = 0; b < tempBondList.size(); b++)
        {
            removeBond(tempBondList[b].bondTag);
        }
        
        // remove angles associated with this particle
        std::vector <struct Angle> tempAngleList = getAngleTagsByIndex(tempIndex);
        for(int a = 0; a < tempAngleList.size(); a++)
        {
            removeAngle(tempAngleList[a].angleTypeIndex);
        }
        
        // update neighbors to not include this particle
        for(int i = 0; i < v_neighborTags[tempIndex].size(); i++)
        {
            int tempNeighborTag = v_neighborTags[tempIndex][i];
            int tempNeighborIndex = tagVector.getIndexOfTag(tempNeighborTag);
            for(int j = 0; j < v_neighborTags[tempNeighborIndex].size(); j++)
            {
                if(particleTag == v_neighborTags[tempNeighborIndex][j])
                {
                    v_neighborTags[tempNeighborIndex][j] = v_neighborTags[tempNeighborIndex].back();
                    v_neighborTags[tempNeighborIndex].pop_back();
                    break;
                }
            }
        }
        
		v_pos[tempIndex] = v_pos.back();
		v_force[tempIndex] = v_force.back();
		v_mass[tempIndex] = v_mass.back();
		v_bins[tempIndex] = v_bins.back();
		v_bondTags[tempIndex] = v_bondTags.back();
        v_angleTags[tempIndex] = v_angleTags.back();
        v_beadToFilament[tempIndex] = v_beadToFilament.back();
        v_diagonalStress[tempIndex] = v_diagonalStress.back();
        v_neighborTags[tempIndex] = v_neighborTags.back();
        v_viscosityType[tempIndex] = v_viscosityType.back();
		
        tagVector.remove(particleTag);
		
		v_pos.pop_back();
		v_force.pop_back();
		v_mass.pop_back();
		v_bins.pop_back();
		v_bondTags.pop_back();
        v_angleTags.pop_back();
        v_beadToFilament.pop_back();
        v_diagonalStress.pop_back();
        v_neighborTags.pop_back();
        v_viscosityType.pop_back();
	}
	else
	{
		cout << "Could not find particle tag " << particleTag << endl;
		exit(0);
	}
}

void ParticleInfo::setTagAtPos(int pos, int newTag)
{
    tagVector.setTagAtPos(pos, newTag);
}

int ParticleInfo::getCurrMaxTag()
{
    return tagVector.getCurrMaxTag();
}

struct Coordinate ParticleInfo::getPos(int particleTag)
{
    int tempIndex = tagVector.getIndexOfTag(particleTag);
    
	return getPosByIndex(tempIndex);
}

std::vector <Coordinate> ParticleInfo::getAllPosByIndex()
{
	return v_pos;
}

struct Coordinate ParticleInfo::getPosByIndex(int particleIndex)
{
    if(particleIndex < 0 || particleIndex > numParticles)
    {
        throw std::runtime_error("Attempted to get position of particle with index " + std::to_string(particleIndex));
    }
    
	return v_pos[particleIndex];
}

void ParticleInfo::setPos(int particleTag, struct Coordinate c)
{
	v_pos[tagVector.getIndexOfTag(particleTag)] = c;
}

void ParticleInfo::setPosByIndex(int particleIndex, struct Coordinate c)
{
	v_pos[particleIndex] = c;
}

struct Coordinate ParticleInfo::getForce(int particleTag)
{
	return v_force[tagVector.getIndexOfTag(particleTag)];
}
struct Coordinate ParticleInfo::getForceByIndex(int particleIndex)
{
	return v_force[particleIndex];
}
void ParticleInfo::setForce(int particleTag, struct Coordinate c)
{
	v_force[tagVector.getIndexOfTag(particleTag)] = c;
}
void ParticleInfo::setForceByIndex(int particleIndex, struct Coordinate c)
{
	v_force[particleIndex] = c;
}
void ParticleInfo::addForce(int particleTag, struct Coordinate c)
{
    v_force[tagVector.getIndexOfTag(particleTag)]  = c + v_force[tagVector.getIndexOfTag(particleTag)];
}
void ParticleInfo::addForceByIndex(int particleIndex, struct Coordinate c)
{    
    v_force[particleIndex]  = c + v_force[particleIndex];
}

int ParticleInfo::getBinByIndex(int particleIndex)
{
	return v_bins[particleIndex];
}

void ParticleInfo::setBinByIndex(int particleIndex, int bin)
{
	v_bins[particleIndex] = bin;
}

int ParticleInfo::getIndexOfTag(int tag)
{
	return tagVector.getIndexOfTag(tag);
}

int ParticleInfo::getTagAtIndex(int pos)
{
	return tagVector.getTagAtIndex(pos);
}

int ParticleInfo::getNumParticles()
{
	return numParticles;
}

bool ParticleInfo::checkTagExists(int tag)
{
    return tagVector.checkTagExists(tag);
}


std::vector <struct HalfBond> ParticleInfo::getBondTagsByIndex(int particleIndex)
{
	return v_bondTags[particleIndex];
}

std::vector <struct HalfBond> ParticleInfo::getBondTags(int particleTag)
{
	return v_bondTags[tagVector.getIndexOfTag(particleTag)];
}

//checks if any bond exists between b.i and b.j
bool ParticleInfo::checkBondExists(struct Bond b)
{
    int index = tagVector.getIndexOfTag(b.i);
    //v_bondTags only stores the tag of the bonded particle and not the bond type, may want to include this as a new struct HalfBond
    for(int i = 0; i < v_bondTags[index].size(); i++)
    {
        if(v_bondTags[index][i].j == b.j)
            return true;
    }
    return false;
}

/*void ParticleInfo::addBondTagByIndex(int particleIndex, int newBondTag)
{
	v_bondTags[particleIndex].push_back(newBondTag);
}

void ParticleInfo::addBondTag(int particleTag, int newBondTag)
{
	v_bondTags[v_tag[particleTag]].push_back(newBondTag);
}*/
std::vector <struct Angle> ParticleInfo::getAngleTagsByIndex(int particleIndex)
{
	return v_angleTags[particleIndex];
}
std::vector <struct Angle> ParticleInfo::getAngleTags(int particleTag)
{
	return v_angleTags[tagVector.getIndexOfTag(particleTag)];
}

int ParticleInfo::addAngle(struct Angle newAngle)
{
    v_angles.push_back(newAngle);
    int newAngleTag = angleTagVector.add();
    
    int particleITag = newAngle.i;
    int particleJTag = newAngle.j;
    int particleKTag = newAngle.k;
    
    v_angleTags[tagVector.getIndexOfTag(particleITag)].emplace_back(particleITag, particleJTag, particleKTag, newAngleTag);
    v_angleTags[tagVector.getIndexOfTag(particleJTag)].emplace_back(particleITag, particleJTag, particleKTag, newAngleTag);
    v_angleTags[tagVector.getIndexOfTag(particleKTag)].emplace_back(particleITag, particleJTag, particleKTag, newAngleTag);
    
    return newAngleTag;
}

int ParticleInfo::addBond(struct Bond newBond)
{
	v_bonds.push_back(newBond);
    std::vector <int> newNeighbors;
    v_bondNeighborTags.push_back(newNeighbors);
    
    int newBondTag = bondTagVector.add();
    
    int particleITag = newBond.i;
    int particleJTag = newBond.j;
    
    int particleIIndex = tagVector.getIndexOfTag(particleITag);
    int particleJIndex = tagVector.getIndexOfTag(particleJTag);
    
    if(particleIIndex < 0 || particleJIndex < 0)
    {
        std::cout << "Could not find particle. " << particleITag << " " << particleJTag << " " << particleIIndex << " " << particleJIndex << std::endl;
        exit(0);
    }
    // place half-bonds associated with newBond associated with both particles in newBond
    v_bondTags[particleIIndex].emplace_back(particleJTag, newBondTag);
    v_bondTags[particleJIndex].emplace_back(particleITag, newBondTag);
    
    return newBondTag;
}

struct Angle ParticleInfo::getAngle(int aTag)
{
    int aIndex = angleTagVector.getIndexOfTag(aTag);
    /*if(aIndex < 0 || aIndex > v_angles.size())
    {
        cout << "error in getAngle: " << aIndex << " " << aTag << " " << v_angles.size() << endl;
        exit(0);
    }*/
    
    return this->getAngleByIndex(aIndex);
}

struct Angle ParticleInfo::getAngleByIndex(int aIndex)
{
    if(aIndex < 0 || aIndex > v_angles.size())
    {
        cout << "error in getAngleByIndex: " << aIndex << " " << v_angles.size() << endl;
        exit(0);
    }
    
    return v_angles[aIndex];
}

struct Bond ParticleInfo::getBond(int bTag)
{
    int bIndex = bondTagVector.getIndexOfTag(bTag);
    if(bIndex < 0 || bIndex > v_bonds.size())
    {
        cout << "error in getBond: " << bIndex << " " << bTag << " " << v_bonds.size() << endl;
        exit(0);
    }
    
    return this->getBondByIndex(bIndex);
}

struct Bond ParticleInfo::getBondByIndex(int bIndex)
{
    if(bIndex < 0 || bIndex > v_bonds.size())
    {
        cout << "error in getBondByIndex: " << bIndex << " " << v_bonds.size() << endl;
        exit(0);
    }
    
    return v_bonds[bIndex];
}

void ParticleInfo::setBondType(int bTag, int newType)
{
    int bIndex = bondTagVector.getIndexOfTag(bTag);
    v_bonds[bIndex].bondTypeIndex = newType;
}

int ParticleInfo::getNumBonds()
{
	return v_bonds.size();
}

int ParticleInfo::getNumAngles()
{
    return v_angles.size();
}

int ParticleInfo::getBondTagAtPos(int bPos)
{
    return bondTagVector.getTagAtIndex(bPos);
}

int ParticleInfo::getBondPosOfTag(int bTag)
{
    return bondTagVector.getIndexOfTag(bTag);
}

void ParticleInfo::removeAngle(int aTag)
{
    int aIndex = angleTagVector.getIndexOfTag(aTag);
    this->removeAngleByIndex(aIndex);
}
void ParticleInfo::removeAngleByIndex(int aIndex)
{
    if(aIndex >= v_angles.size() || aIndex < 0)
    {
        cout << "error in removing bond: " << aIndex << " " << v_angles.size() << endl;
        exit(0);
    }
    
    int beadI = v_angles[aIndex].i;
    int beadJ = v_angles[aIndex].j;
    int beadK = v_angles[aIndex].k;
    
    // need to loop through to find the bond connecting beadI and beadJ and remove it
    bool found = false;
    int currAngleTag = angleTagVector.getTagAtIndex(aIndex);
    
    int indexI = tagVector.getIndexOfTag(beadI);
    for(int i = v_angleTags[indexI].size()-1; i > -1; i--)
    {
        if(v_angleTags[indexI][i].angleTypeIndex == currAngleTag)
        {
            // remove this entry from v_angleTags[indexI]
            v_angleTags[indexI][i] = v_angleTags[indexI].back();
            v_angleTags[indexI].pop_back();
            
            found = true;
            break;
        }
    }
    if(found == false)
        throw std::runtime_error("Could not find angle between ijk: " + std::to_string(beadI) + " " + std::to_string(beadJ) + " " + std::to_string(beadK));
    
    int indexJ = tagVector.getIndexOfTag(beadJ);
    for(int i = v_angleTags[indexJ].size()-1; i > -1; i--)
    {
        if(v_angleTags[indexJ][i].angleTypeIndex == currAngleTag)
        {
            // remove this entry from v_angleTags[indexI]
            v_angleTags[indexJ][i] = v_angleTags[indexJ].back();
            v_angleTags[indexJ].pop_back();
            
            found = true;
            break;
        }
    }
    
    int indexK = tagVector.getIndexOfTag(beadK);
    for(int i = v_angleTags[indexK].size()-1; i > -1; i--)
    {
        if(v_angleTags[indexK][i].angleTypeIndex == currAngleTag)
        {
            // remove this entry from v_angleTags[indexI]
            v_angleTags[indexK][i] = v_angleTags[indexK].back();
            v_angleTags[indexK].pop_back();
            
            found = true;
            break;
        }
    }
    
    // remove from v_angles
    v_angles[aIndex] = v_angles.back();
	v_angles.pop_back();
    
    // remove from angleTagVector
    angleTagVector.remove(currAngleTag);
}

void ParticleInfo::removeBond(int bTag)
{
    int bIndex = bondTagVector.getIndexOfTag(bTag);
	this->removeBondByIndex(bIndex);
}

// check this again carefully!
void ParticleInfo::removeBondByIndex(int bIndex)
{
    if(bIndex >= v_bonds.size() || bIndex < 0)
    {
        cout << "error in removing bond: " << bIndex << " " << v_bonds.size() << endl;
        exit(0);
    }
    
    
    int beadI = v_bonds[bIndex].i;
    int beadJ = v_bonds[bIndex].j;
    
    // need to loop through to find the bond connecting beadI and beadJ and remove it
    bool found = false;
    int indexI = tagVector.getIndexOfTag(beadI);
    for(int i = v_bondTags[indexI].size()-1; i > -1; i--)
    {
        if(v_bondTags[indexI][i].j == beadJ)
        {
            // remove this entry from v_bondTags[indexI]
            v_bondTags[indexI][i] = v_bondTags[indexI].back();
            v_bondTags[indexI].pop_back();
            
            found = true;
            break;
        }
    }
    
    if(found == false)
        throw std::runtime_error("Could not find bond between ij: " + std::to_string(beadI) + " " + std::to_string(beadJ));
    
    found = false;
    int indexJ = tagVector.getIndexOfTag(beadJ);
    for(int i = v_bondTags[indexJ].size()-1; i > -1; i--)
    {
        if(v_bondTags[indexJ][i].j == beadI)
        {
            // remove this entry from v_bondTags[beadI]
            v_bondTags[indexJ][i] = v_bondTags[indexJ].back();
            v_bondTags[indexJ].pop_back();
            
            found = true;
            break;
        }
    }
    
    if(found == false)
        throw std::runtime_error("Could not find bond between ji: " + std::to_string(beadJ) + " " + std::to_string(beadI));
    
    v_bonds[bIndex] = v_bonds.back();
	v_bonds.pop_back();
    
    // this works with bondTagVector which hasnt been updated yet to remove bond so this line is still okay
    int currBondTag = getBondTagAtPos(bIndex);
    
    if(bIndex >= v_bondNeighborTags.size() || bIndex < 0)
    {
        cout << "error in updating neighbors : " << currBondTag << " " << v_bondNeighborTags.size() << endl;
        exit(0);
    }
    
    //need to also update neighborTags that had bond at bIndex as a neighbor
    for(int bt = 0; bt < v_bondNeighborTags[bIndex].size(); bt++)
    {
        int currNeighborTag = v_bondNeighborTags[bIndex][bt];
        
        int tempBondIndex = getBondPosOfTag(currNeighborTag);
        
        if(tempBondIndex < 0 || tempBondIndex >= v_bondNeighborTags.size())
        {
            cout << "tempBondIndex too large: " << tempBondIndex << " " << v_bondNeighborTags.size() << endl;
            exit(0);
        }
        
        //look for currBondTag in bondNeighboringTags (it might not be in there though since the bonds are only one sided???)
        for(int j = v_bondNeighborTags[tempBondIndex].size()-1; j > -1; j--)
        {
            if(v_bondNeighborTags[tempBondIndex][j] == currBondTag)
            {
                v_bondNeighborTags[tempBondIndex][j] = v_bondNeighborTags[tempBondIndex].back();
                v_bondNeighborTags[tempBondIndex].pop_back();
                break;
            }
        }
    }
    
    v_bondNeighborTags[bIndex] = v_bondNeighborTags.back();
	v_bondNeighborTags.pop_back();
    
    bondTagVector.remove(currBondTag);
    
    // remove any angles which have i and j adjacent to each other (not i-v-j)
    for(int i = v_angleTags[indexI].size()-1; i > -1; i--)
    {
        if((v_angleTags[indexI][i].i == beadI && v_angleTags[indexI][i].j == beadJ) || (v_angleTags[indexI][i].j == beadI && v_angleTags[indexI][i].i == beadJ) || (v_angleTags[indexI][i].j == beadI && v_angleTags[indexI][i].k == beadJ) || (v_angleTags[indexI][i].k == beadI && v_angleTags[indexI][i].j == beadJ))
        {
            // remove this entry from v_angleTags[indexI]
            int angleTag = v_angleTags[indexI][i].angleTypeIndex;
            
            removeAngle(angleTag);
            // try to remove angleTag from v_angles if it exists (i.e. hasnt been remove already) (check angleTagVector)
            /*if(angleTagVector.checkTagExists(angleTag))
            {
                angleTagVector.remove(angleTag);
            }
            
            // remove this angle from angleTags
            v_angleTags[indexI][i] = v_angleTags[indexI].back();
            v_angleTags[indexI].pop_back();
            
            for(int i = v_angleTags[indexJ].size()-1; i > -1; i--)
            {
                if(v_angleTags[indexJ][i].angleTypeIndex == angleTag)
                {
                    // remove angle with same angleTag as original angle removed
                    v_angleTags[indexJ][i] = v_angleTags[indexJ].back();
                    v_angleTags[indexJ].pop_back();
                    
                    break;
                }
            }
            
            int beadK = v_angleTags[indexI][i].k;
            if(beadK == beadJ || beadK == beadI)
            {
                beadK = v_angleTags[indexI][i].j;
                
                if(beadK == beadJ || beadK == beadI)
                {
                    beadK = v_angleTags[indexI][i].i;
                }
            }
            int indexK = tagVector.getIndexOfTag(beadK);
            
            for(int i = v_angleTags[indexK].size()-1; i > -1; i--)
            {
                if(v_angleTags[indexK][i].angleTypeIndex == angleTag)
                {
                    // remove angle with same angleTag as original angle removed
                    v_angleTags[indexK][i] = v_angleTags[indexK].back();
                    v_angleTags[indexK].pop_back();
                    
                    break;
                }
            }*/
        }
    }    
}

std::vector <int> & ParticleInfo::getBondNeighborTagsByIndex(int bondIndex)
{
    if(bondIndex < 0 || bondIndex >= v_bondNeighborTags.size())
    {
        throw std::runtime_error("Error in getBondNeighborTagsByIndex: " + std::to_string(bondIndex) + ", " + std::to_string(v_bondNeighborTags.size()));
    }
    
    return v_bondNeighborTags[bondIndex];
}

std::vector <int> & ParticleInfo::getBondNeighborTags(int bondTag)
{
    int tempIndex = getBondPosOfTag(bondTag);
    return getBondNeighborTagsByIndex(tempIndex);
}

void ParticleInfo::setBondNeighborTags(int bondTag, std::vector <int> newNeighbors)
{
    int tempIndex = getBondPosOfTag(bondTag);
    setBondNeighborTagsByIndex(tempIndex, newNeighbors);
}
void ParticleInfo::setBondNeighborTagsByIndex(int bondIndex, std::vector <int> newNeighbors)
{
    v_bondNeighborTags[bondIndex] = newNeighbors;
}

void ParticleInfo::appendBondNeighborTags(int bondTag, std::vector <int> additionalNeighbors)
{
    int tempIndex = getBondPosOfTag(bondTag);
    appendBondNeighborTagsByIndex(tempIndex, additionalNeighbors);
}
void ParticleInfo::appendBondNeighborTagsByIndex(int bondIndex, std::vector <int> additionalNeighbors)
{
    // need to concatenate current neighbors and additionalNeighbors to form new neighbor list
    // https://stackoverflow.com/questions/201718/concatenating-two-stdvectors
    std::vector <int> org_neighbors = v_bondNeighborTags[bondIndex];
    v_bondNeighborTags[bondIndex].insert(org_neighbors.end(), additionalNeighbors.begin(), additionalNeighbors.end());
}

void ParticleInfo::appendBondNeighborTag(int bondTag, int additionalNeighbors)
{
    int tempIndex = getBondPosOfTag(bondTag);
    appendBondNeighborTagByIndex(tempIndex, additionalNeighbors);
}
void ParticleInfo::appendBondNeighborTagByIndex(int bondIndex, int additionalNeighbors)
{
    v_bondNeighborTags[bondIndex].push_back(additionalNeighbors);
}
    
void ParticleInfo::resetBondNeighborTagsByIndex(int bondIndex, std::vector <int> possibleNeighborBondTypes)
{
    Bond masterBond = getBondByIndex(bondIndex);
    //if(std::find(possibleNeighborBondTypes.begin(), possibleNeighborBondTypes.end(), masterBond.bondTypeIndex) != possibleNeighborBondTypes.end())
    {
        int bondTag = getBondTagAtPos(bondIndex);
        
        int tagI = masterBond.i;
        int tagJ = masterBond.j;
        
        std::vector <int> iNeighbors = getNeighbors(tagI);
        std::vector <int> jNeighbors = getNeighbors(tagJ);
        
        //https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
        //allNeighbors is a set which contains all beads which are either a neighbor of tagJ or tagI
        std::set <int> allNeighbors;
        for(int m = 0; m < iNeighbors.size(); m++)
            allNeighbors.insert(iNeighbors[m]);
        for(int m = 0; m < jNeighbors.size(); m++)
            allNeighbors.insert(jNeighbors[m]);
        
        // now loop through allNeighbors to find bond tags associated with these neighbors
        std::set <int> bondNeighborTags;
        
        for(std::set<int>::iterator it = allNeighbors.begin(); it != allNeighbors.end(); ++it)
        {
            int currNeighborTagI = *it;
        
            //loop over all bonds connecting to particle currNeighborTagI
            std::vector <struct HalfBond> secondBondList = getBondTags(currNeighborTagI);
            
            for(int loopVar = 0; loopVar < secondBondList.size(); loopVar++)
            {
                HalfBond tempHalfBond = secondBondList[loopVar];
                int currNeighborTagJ = tempHalfBond.j;
                int secondBondTag = tempHalfBond.bondTag;
                
                int secondBondType = getBond(secondBondTag).bondTypeIndex;
                
                bool isNeighboringBond = (tagI == currNeighborTagI) || (tagI == currNeighborTagJ) || (tagJ == currNeighborTagI) || (tagJ == currNeighborTagJ);
                
                // only add bonds as neighbors that are type 0 (filament bonds),
                // and are not neighboring
                //if(!isNeighboringBond && std::find(possibleNeighborBondTypes.begin(), possibleNeighborBondTypes.end(), secondBondType) != possibleNeighborBondTypes.end())
                if(!isNeighboringBond)
                {
                    bondNeighborTags.insert(secondBondTag);
                }
            }
        }
        
        std::vector <int> tempVector;
        tempVector.assign(bondNeighborTags.begin(), bondNeighborTags.end());

        setBondNeighborTagsByIndex(bondIndex, tempVector);
    }
}
int ParticleInfo::getBeadToFilament(int particleTag)
{
	return v_beadToFilament[tagVector.getIndexOfTag(particleTag)];
}
int ParticleInfo::getBeadToFilamentByIndex(int particleIndex)
{
	return v_beadToFilament[particleIndex];
}

void ParticleInfo::setBeadToFilament(int particleTag, int newFilamentIndex)
{
    v_beadToFilament[tagVector.getIndexOfTag(particleTag)] = newFilamentIndex;
}
void ParticleInfo::setBeadToFilamentByIndex(int particleIndex, int newFilamentIndex)
{
    v_beadToFilament[particleIndex] = newFilamentIndex;
}

void ParticleInfo::resetDiagonalStress()
{
    struct Coordinate zeroStress = {0.0, 0.0, 0.0};
    for(int p = 0; p < v_diagonalStress.size(); p++)
    {
        for(int p2 = 0; p2 < v_diagonalStress[p].size(); p2++)
        {
            v_diagonalStress[p][p2] = zeroStress;
        }
    }
}

void ParticleInfo::setNeighbors(int particleTag, std::vector <int> newNeighbors)
{
    v_neighborTags[tagVector.getIndexOfTag(particleTag)] = newNeighbors;
}
void ParticleInfo::setNeighborsByIndex(int particleIndex, std::vector <int> newNeighbors)
{
    v_neighborTags[particleIndex] = newNeighbors;
}
std::vector <int> ParticleInfo::getNeighbors(int particleTag)
{
    return v_neighborTags[tagVector.getIndexOfTag(particleTag)];
}

void ParticleInfo::addNeighbor(int particleTag, int newNeighbor)
{
    // need to check that particleTag still exists (tagVector.getIndexOfTag(particleTag) != -1)
    // grid does not know if particles have been removed!!!
    // now grid removed
    int currPos = tagVector.getIndexOfTag(particleTag);
    if(currPos >= 0)
    {
        v_neighborTags[tagVector.getIndexOfTag(particleTag)].push_back(newNeighbor);
    }
    else
    {
        cout << particleTag << " " << currPos << " " << newNeighbor << endl;
    }
}

struct Coordinate ParticleInfo::getStress(int particleTag, int stressIndex)
{
    return v_diagonalStress[tagVector.getIndexOfTag(particleTag)][stressIndex];
}
struct Coordinate ParticleInfo::getStressByIndex(int particleIndex, int stressIndex)
{
    return v_diagonalStress[particleIndex][stressIndex];
}
void ParticleInfo::setStress(int particleTag, struct Coordinate newStress, int stressIndex)
{
    v_diagonalStress[tagVector.getIndexOfTag(particleTag)][stressIndex] = newStress;
}
void ParticleInfo::setStressByIndex(int particleIndex, struct Coordinate newStress, int stressIndex)
{
    v_diagonalStress[particleIndex][stressIndex]  = newStress;
}
void ParticleInfo::addStress(int particleTag, struct Coordinate newStress, int stressIndex)
{
    v_diagonalStress[tagVector.getIndexOfTag(particleTag)][stressIndex]  = newStress + v_diagonalStress[tagVector.getIndexOfTag(particleTag)][stressIndex] ;
}
void ParticleInfo::addStressByIndex(int particleIndex, struct Coordinate newStress, int stressIndex)
{
    v_diagonalStress[particleIndex][stressIndex]  = newStress + v_diagonalStress[particleIndex][stressIndex] ;
}

int ParticleInfo::getViscosityTypeByIndex(int particleIndex)
{
    return v_viscosityType[particleIndex];
}
void ParticleInfo::setViscosityTypeByIndex(int particleIndex, int newViscosityType)
{
    v_viscosityType[particleIndex] = newViscosityType;
}

// finds tag between particles aTag and bTag
int ParticleInfo::findBondTag(int aTag, int bTag)
{
    int tempBondTag = -1;
    std::vector <HalfBond> tempBondList = this->getBondTags(aTag);
    for(int s = 0; s < tempBondList.size(); s++)
    {
        if(tempBondList[s].j == bTag)
        {
            return tempBondList[s].bondTag;
        }
    }
    
    cout << "Error in findBondTag: " << aTag << ", " << bTag << endl;
    return -1;
}
