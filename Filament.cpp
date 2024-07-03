#include "Filament.h"

using namespace std;

Filament::Filament()
{
	isDaughter = false;
	motherTag = -1;
    
    cappedBarbedEnd = false;
    cappedPointedEnd = false;
}

int Filament::getNumParticles()
{
	return p_tags.size();
}

int Filament::addParticleFront(int newTag)
{
    // add to pointed end
	p_tags.push_front(newTag);
		
	if(p_tags.size() > 1)
	{
		int oldFront = p_tags[1];
		return oldFront;
	}
	else
	{
		return -1;
	}
}

int Filament::addParticleBack(int newTag)
{
    // add to barbed end
	p_tags.push_back(newTag);
	
	if(p_tags.size() > 1)
	{
		int oldBack = p_tags[p_tags.size()-2];
		return oldBack;
	}
	else
	{
		return -1;
	}	
}

int Filament::getTagAtIndex(int index)
{
	if(index < p_tags.size())
	{
		return p_tags[index];
	}
	else
	{
		return -1;
	}
}

int Filament::getPointedTag()
{
    return p_tags.front();
}

int Filament::getBarbedTag()
{
    return p_tags.back();
}

void Filament::removeParticleFront()
{
	p_tags.pop_front();
}

void Filament::removeParticleBack()
{
	p_tags.pop_back();
}

// dont call this, use simBox to calculate distances
struct Coordinate Filament::getFrontDirection(ParticleInfo pinfo)
{
	struct Coordinate c1 = pinfo.getPos(p_tags.front());
	struct Coordinate c2 = pinfo.getPos(p_tags[1]);
	
	struct Coordinate c3;
	
	c3.x = c1.x - c2.x;
	c3.y = c1.y - c2.y;
	c3.z = c1.z - c3.z;
	
	c3 = c3.getUnitCoord();
	
	return c3;
}

// this is wrong!
struct Coordinate Filament::matrixProduct(double basis[3][3], struct Coordinate a)
{
    cout << "MATRIX PRODUCT IS WRONG?" << endl;
	Coordinate c = {0.0, 0.0, 0.0};
	
	for(int j = 0; j < 3; j++)
	{
		c.x += basis[0][j] * a.x;
	}
	for(int j = 0; j < 3; j++)
	{
		c.y += basis[0][j] * a.y;
	}
	for(int j = 0; j < 3; j++)
	{
		c.z += basis[0][j] * a.z;
	}
	
	return c;
}

void Filament::setIsDaughter(bool state)
{
	isDaughter = state;
}

bool Filament::getIsDaughter()
{
	return isDaughter;
}

void Filament::setMotherTag(int newMotherTag)
{
	motherTag = newMotherTag;
    
    if(motherTag >= 0)
        isDaughter = true;
}

int Filament::getMotherTag()
{
	return motherTag;
}

/*void Filament::setDaughterFilamentLength(int newLength)
{
	totalDaughterLength = newLength;
}

int Filament::getDaughterFilamentLength()
{
	return totalDaughterLength;
}

void Filament::addDaughterFilamentIndex(int filamentIndex)
{
	v_daughterFilamentIndexes.push_back(filamentIndex);
}

std::vector <int> Filament::getDaughterFilamentIndexes()
{
	return v_daughterFilamentIndexes;
}*/

// splits filament between pos and pos+1 beads and returns new filament which is from 0 to pos
// i.e. breaks into two filaments as -(pos-1)-(pos) [break] (pos+1)-(pos+2)-
// TODO: recalculate totaldaughterlength and daughterFilamentIndexes for both this and newFilament
Filament Filament::breakFilamentAtPos(int pos)
{
	int orgNumParticles = p_tags.size();
	Filament newFil;
    
    // if not in the right range can't break filament between pos and pos+1
    if(pos < 0 || pos >= orgNumParticles)
    {
        throw std::runtime_error("Could not break filament at pos "  + std::to_string(pos) + ", # particles: " + std::to_string(orgNumParticles));
    }
		
    for(int i = 0; i <= pos; i++)
    {
        int tempKey = p_tags.front();
        
        newFil.addParticleBack(tempKey);
        
        p_tags.pop_front();
    }
    
    newFil.setIsDaughter(false);
	
	return newFil;
}

// splits filament into two filaments 0-(pos-1) and (pos+1)-end and returns new filament from 0-(pos-1)
Filament Filament::removeBeadAtPos(int pos)
{
    Filament newFil = breakFilamentAtPos(pos);
    
    newFil.removeParticleBack();
	
	return newFil;
}

Filament Filament::removeBeadByTag(int tag)
{
    int posOfTag = -1;
    for(int i = 0; i < p_tags.size(); i++)
    {
        if(p_tags[i] == tag)
        {
            posOfTag = i;
            break;
        }
    }
    
    if(posOfTag >= 0)
    {
        return removeBeadAtPos(posOfTag);
    }
    else
    {
        throw std::runtime_error("Could not find " + std::to_string(tag) + " in filament, removeBeadByTag");
    }
}

std::deque <int> Filament::getAllTags()
{
    return p_tags;
}

void Filament::setCappedBarbedEnd(bool newState)
{
    cappedBarbedEnd = newState;
}
bool Filament::getCappedBarbedEnd()
{
    return cappedBarbedEnd;
}

void Filament::setCappedPointedEnd(bool newState)
{
    cappedPointedEnd = newState;
}
bool Filament::getCappedPointedEnd()
{
    return cappedPointedEnd;
}

int Filament::getIndexOfTag(int tempTag)
{
    for(int i = 0; i < p_tags.size(); i++)
    {
        if(p_tags[i] == tempTag)
            return i;
    }
    
   return -1;
}