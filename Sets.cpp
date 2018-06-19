
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
		enter.dirty = false;
		enter.valid = false;
		enter.tag = 0;
		ways_.push_back(enter);
		LRU_arr_.push_back(i);//initializing LRU
	}
}

/* write if doesnt exist for read operation*/
bool Cache_set::clearSet(unsigned address,unsigned tag, bool L1_or_L2, unsigned* tag_to_evict)
{
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++)
	{
		if(it->tag == tag && it->valid) {
			return true;
		}
	}
	//none is with the same tag, and someone is empty
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++)
	{
		if(!(it->valid)) {
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
		it->dirty = false;
		it->valid = false;
		return false;//requirs access to L1 to remove tag
	}
	else//L1-- return false if evict dirty
	{
		if (it->dirty == 1)//need to evict a dirty
		{
			*tag_to_evict = it->tag;
			it->dirty = false;
			it->valid = false;
			return false;//evicted the page but signals that should write modified to L2
		}
		else
		{
			it->dirty = false;
			it->valid = false;
			return true;
		}
	}
}


bool Cache_set::removetag(unsigned tag)
{
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++)
	{
		if(it->tag == tag && it->valid) {
			bool tmp = it->dirty;
			it->dirty = false;
			it->tag = 0;
			it->valid = false;
			return tmp;
		}
	}
	return false;
}

/* Name: write2Set, for write operation
Descriptioin: writing to the set, update the LRU list(calls for updateLRU func.),change dirty bit
parameters: tag 
return value: 
*/
bool Cache_set::write2Set(unsigned tag, unsigned address, bool real_write, bool dirty, bool L1_or_L2)
{
	int k=0;
	//if tag is valid-> great
	for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++,k++)
	{
		if(it->tag == tag && it->valid) {
			it->dirty = true;
			updateLRU(k);
			return true;
		}
	}
	//else- find empty space
	k=0;
	if(!real_write)
	{
		for(std::list<Map>::iterator it=ways_.begin(); it != ways_.end();it++,k++)
		{
			if(it->valid == false) {
				it->tag = tag;
				it->dirty = dirty;
				it->valid = true;
				updateLRU(k);
				return true;
			}
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
		if(it->tag == tag && it->valid) {
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

