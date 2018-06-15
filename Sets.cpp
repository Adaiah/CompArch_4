
#include "Sets.h"

/* Name: C'tr of the class
Descriptioin: creates an instantiation of a set
parameters: ways- number of ways;
return value: none
*/
Cache_set::Cache_set(unsigned ways)
{
	for (unsigned i = 0; i < ways; i++)
	{
		Map enter;
		enter.dirty = 0;
		enter.valid = false;
		enter.tag = -1;
		ways_.push_back(enter);
		LRU_arr_.push_back(i);//initializing LRU
	}
}

/* write if doesnt exist for read operation*/
bool Cache_set::add2Set(unsigned tag, bool wa, bool L1_or_L2, unsigned* tag_to_evict)
{
	int k=0;
	//none is with the same tag, and someone is empty
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++,k++)
	{
		if(!(it->valid)) {
			it->dirty = 0;
			it->valid = true;
			it->tag = tag;
			updateLRU(k);
			ASDF("valid")
			return true;
		}
	}
	//all ways are occupied
	unsigned lru = lru_from_LRU();
	std::list<Map>::iterator it=ways_.begin();
	for (unsigned i = 0; i < lru; ++i)
	{
		it++;
	}
	if(L1_or_L2 == L2T)//guarenteed no tag exists
	{
		*tag_to_evict = it->tag;
		it->dirty = 0;
		it->valid = true;
		it->tag = tag;
		updateLRU(lru);
		return false;//requirs access to L1 to remove tag
	}
	else//L1
	{
		if (it->dirty == 1)//need to evict a dirty
		{
			it->dirty = 0;
			it->valid = true;
			it->tag = tag;
			updateLRU(lru);
			return false;
		}
		else
		{
			it->dirty = 0;
			it->valid = true;
			it->tag = tag;
			updateLRU(lru);
			return true;
		}
	}
}


void Cache_set::removetag(unsigned tag)
{
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++)
	{
		if(it->tag == tag) {
			it->dirty = 0;
			it->tag = 0;
			it->valid = false;
			return;
		}
	}
}

/* Name: write2Set, for write operation
Descriptioin: writing to the set, update the LRU list(calls for updateLRU func.),change dirty bit
parameters: tag 
return value: 
*/
bool Cache_set::write2Set(unsigned tag)
{
	int k=0;
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++,k++)
	{
		if(it->tag == tag && it->valid) {
			it->dirty = 1;

			updateLRU(k);
			return true;
		}
	}
	return false;
}

/* Name: readSet
Descriptioin: reading from the set, checking dirty bit for miss, update LRU(calls for updateLRU func.)
parameters: tag 
return value: true- if hit
				false if miss
*/
bool Cache_set::readSet(unsigned tag)
{
	int k=0;
	for(std::list<Map>::iterator it=ways_.begin();it!=ways_.end();it++,k++)
	{
		if(it->tag == tag && it->valid) {//should we care about dirty????????????
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
void Cache_set::updateLRU(unsigned MRU)
{
	for(std::list<unsigned>::iterator it=LRU_arr_.begin();it != LRU_arr_.end(); it++)
	{
		if (*it == MRU)
		{
			LRU_arr_.erase(it);
			LRU_arr_.push_back(MRU);
			return;
		}
	}
}

unsigned Cache_set::lru_from_LRU()
{
	std::list<unsigned>::iterator it=LRU_arr_.begin();
	return *(it);
}


Cache_set::~Cache_set() {
}

