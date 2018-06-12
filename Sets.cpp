
#include "Sets.h"

/* Name: C'tr of the class
Descriptioin: creates an instantiation of a set
parameters: ways- number of ways;
return value: none
*/
Cache_set::Cache_set(unsigned ways)
{
	for (std::list<Map>::iterator i = ways_.begin(), int j=0; i !=ways_.end(); i++,j++)
	{
		Map enter;
		enter.dirty = 0;
		enter.tag = 0;
		ways_.insert(i, enter);
		*(LRU_arr_.begin() +j) = -1;//initializing LRU
	}
}

/* Name: write2Set
Descriptioin: writing to the set, update the LRU list(calls for updateLRU func.),change dirty bit
parameters: tag 
return value: true- if hit
				false if miss
*/
bool write2Set(unsigned tag);

/* Name: readSet
Descriptioin: reading from the set, checking dirty bit for miss, update LRU(calls for updateLRU func.)
parameters: tag 
return value: true- if hit
				false if miss
*/
bool Cache_set::readSet(unsigned tag)
{
	unsigned int k=0;
	for(std::list<Map>::iterator it=ways_.begin();it!=ways_.end();it++,k++)
	{
		if(it->tag==tag && it->dirty==1) {
			
			updateLRU(k);
			return true;
		}
		return false;
	}
}


/* Name: updateLRU
Descriptioin: update the list holding LRU. remove the MRU and add to the end of the list and removes data from the way in the set
	LRU is at the top of the list
parameters: MRU- most recently used way
return value: none
*/
void Cache_set::updateLRU(unsigned MRU)
{
	LRU_arr_.erase(LRU_arr_.begin+MRU);
	LRU_arr_.push_back(MRU);
}


/* Name: find_tag
Descriptioin: finds the location of a tag in the set
parameters: tag
return value: the way the tag was found at
*/
int Cache_set::find_tag(unsigned tag) {

}


Cache_set::~Cache_set() {
}

