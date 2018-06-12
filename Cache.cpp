#include "Cache.h"

/* Name: C'tr of the class
   Descriptioin: creates an instantiation of a cache
   parameters: Csize- cache size in bits;
				Bsize- block size in bits		
				Assoc - # of ways 
				Writing_policy - if allocating or not
	return value: none
*/
cache::cache(unsigned Csize, unsigned cache_cyc, unsigned Bsize, unsigned Assoc, unsigned Cyc, char WrAlloc) :
	cache_size_(Csize), cache_cycle_(cache_cyc),Bsize_(Bsize), Assoc_(Assoc), miss_(0),
	writing_policy_(WrAlloc), time_(0), numOfAccess_(0) {
	level_ = level++;
	for (int i=0; i<cache_size_;i++)
	{
		Cache_set enter= Cache_set(Assoc);
		sets.push_back(enter);
	}
}
/* Name: DC'tr of the class
   Descriptioin:N/A
   parameters: None
	return value: None
*/
cache::~cache(){}

/* Name: get_miss
   Descriptioin: returns the number of miss when using the cahce
				advances the numOfAccess counter
   parameters: None
   return value: miss
*/
unsigned cache::get_miss() const{
return miss_;
}

/* Name: write2cache
   Descriptioin: writes to cahce and according to the writing polilcy of the cache									advances the numOfAccess counter
				advances the numOfAccess counter
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
   return value: true - if hit
				false - if miss
*/
bool cache::Write2Cache(unsigned address)
{
	
}

/* Name: ReadCache
   Descriptioin: reads from the cache
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
*/
bool cache::ReadCache(unsigned address){
	
}


/* Name: getnumOfAccess
   Descriptioin: returns number of access to the cache
   parameters: None
   return value: numOfAccess
*/
double cache:: getnumOfAccess()const
{
	return numOfAccess_;
}


/* Name: gettime
   Descriptioin: returns time reading and writing
   parameters: None
   return value: time
*/
int cache:: gettime()const
{
	return time_;
}

/* Name: calc_set
   Descriptioin: calculates set of a certain address for writing/reading
   parameters: addres
   return value: set
*/
unsigned cache::calc_set(unsigned address){
	unsigned set;

}

/* Name: calc_tag
   Descriptioin: calculates tag of a certain address for writing/reading
   parameters: addres
   return value: tag
*/
unsigned cache::calc_tag(unsigned address)
{
	
}




