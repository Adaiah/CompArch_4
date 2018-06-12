
#include "Sets.h"

/* Name: C'tr of the class
Descriptioin: creates an instantiation of a set
parameters: ways- number of ways;
return value: none
*/
Cache_set::Cache_set(unsigned ways)
{
	for (int i = 0; i < ways; i++)
	{
		Map enter = new Map;
		enter.dirty = 0;
		enter.tag = 0;
		ways_.insert(enter);
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
bool readSet(unsigned tag);


/* Name: updateLRU
Descriptioin: update the list holding LRU. remove the MRU and add to the end of the list and removes data from the way in the set
	LRU is at the top of the list
parameters: MRU_way- most recently used way
return value: none
*/
void updateLRU(unsigned MRU)
{
	
}


/* Name: find_tag
Descriptioin: finds the location of a tag in the set
parameters: tag
return value: the way the tag was found at
*/
int find_tag(unsigned tag);


Cache_set::~Cache_set() {
	for (std::list<Map>::iterator it = ways_.begin; it != ways_.end; it++)
	{
		delete *it;
	}
}

