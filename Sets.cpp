
#include "Sets.h"

/* Name: C'tr of the class
Descriptioin: creates an instantiation of a set
parameters: ways- number of ways;
return value: none
*/
Cache_set::Cache_set(uint32_t ways)
{
	for (std::list<Map>::iterator it = ways_.begin(), int j=0; it != ways_.end(); it++,j++)
	{
		Map enter;
		enter.dirty = 0;
		enter.tag = -1;
		ways_.insert(i, enter);
		*(LRU_arr_.begin() + j) = j;//initializing LRU
	}
}

/* Name: write2Set
Descriptioin: writing to the set, update the LRU list(calls for updateLRU func.),change dirty bit
parameters: tag 
return value: true- if hit
				false if miss
*/
bool write2Set(uint32_t tag)
{
	for(std::list<Map>::iterator it=ways_.begin(), int k=0; it != ways_.end();it++,k++)
	{
		if(it->tag == tag) {
			it->dirty = 1;
			it->tag = tag;
			updateLRU(k);
			return true;
		}
	}

	//none is with the same tag, and someone is empty
	for(std::list<Map>::iterator it=ways_.begin(), int k=0; it != ways_.end();it++,k++)
	{
		if(it->tag == -1) {
			it->dirty = 1;
			it->tag = tag;
			updateLRU(k);
			return false;
		}
	}

	//all ways are occupied
	uint32_t lru = lru_from_LRU();
	if ((ways_.begin() + lru)->dirty == 1)
	{
		//evict
	}
	(ways_.begin() + lru)->dirty = 1;
	(ways_.begin() + lru)->tag = tag;
	updateLRU(lru);
	return false;
}

/* Name: readSet
Descriptioin: reading from the set, checking dirty bit for miss, update LRU(calls for updateLRU func.)
parameters: tag 
return value: true- if hit
				false if miss
*/
bool Cache_set::readSet(uint32_t tag)
{
	for(std::list<Map>::iterator it=ways_.begin(), int k=0;it!=ways_.end();it++,k++)
	{
		if(it->tag==tag) {//should he care about dirty????????????
			updateLRU(k);
			return true;
		}
	}
	return false;
}


/* Name: updateLRU
Descriptioin: update the list holding LRU. remove the MRU and add to the end of the list and removes data from the way in the set
	LRU is at the top of the list
parameters: MRU- most recently used way
return value: none
*/
void Cache_set::updateLRU(uint32_t MRU)
{
	for(std::vector<uint32_t>::iterator it=LRU_arr_.begin();it != LRU_arr_.end(); it++)
	{
		if (*it == MRU)
		{
			LRU_arr_.erase(it);
			LRU_arr_.push_back(MRU);
		}
	}
}

uint32_t Cache_set::lru_from_LRU()
{
	return *(LRU_arr_.begin());
}


Cache_set::~Cache_set() {
}

