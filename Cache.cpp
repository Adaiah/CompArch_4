#include "Cache.h"
#define CMDSIZE 32

/*calculates the power of 2 according to the exponent given*/
static unsigned powerOf2(unsigned i)
{
	unsigned result = 1;
	for (unsigned j=0; j < i; j++)
	{
		result *= 2;
	}
	return result;
}

static unsigned extractBits(unsigned num, unsigned from, unsigned to) { // include from and to
	if (to + 1 == CMDSIZE)
	{
		return num / powerOf2(from);
	}
	num = num % powerOf2(to + 1);
	num /= powerOf2(from);
	return num;
}




/* Name: C'tr of the class
   Descriptioin: creates an instantiation of a cache
   parameters: Csize- cache size in bits;
				Bsize- block size in bits		
				Assoc- in bits
				Writing_policy - if allocating or not
	return value: none
*/
cache::cache(unsigned Csize, unsigned cache_cyc, unsigned Bsize, unsigned Assoc, bool WrAlloc, bool L1_or_L2) {
	miss_ = 0;
	time_ = 0;
	numOfAccess_ = 0;
	writing_policy_ = WrAlloc;
	offset_bits_ = Bsize;
	set_bits_ = Csize - Bsize - Assoc;
	tag_bits_ = CMDSIZE - offset_bits_ - set_bits_;
	cache_cycle_ = cache_cyc;
	L1_or_L2_ = L1_or_L2;
	Bsize_ = powerOf2(Bsize);
	cache_size_ = powerOf2(Csize);
	Assoc_ = powerOf2(Assoc);
	sets_ = powerOf2(set_bits_);//maybe we dont have enough space to use all of the power2(set_bits_)????
	for (unsigned i=0; i<sets_;i++)
	{
		Cache_set enter= Cache_set(Assoc_);
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
   Descriptioin: writes to cahce and according to the writing polilcy of the cache
				advances the numOfAccess counter
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
   return value: true - if hit
				false - if miss
*/
bool cache::Write2Cache(unsigned address)
{
	numOfAccess_++;
	unsigned set = extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1);
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	if (sets[set].write2Set(tag))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/* Name: Add2Cache
   Descriptioin: adds data to cache, if in read operation the data isnt there
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
   return value: true - can add without eviction
*/
bool cache::Add2Cache(unsigned address, unsigned* tag_to_evict)
{
	if(tag_to_evict != NULL) numOfAccess_++;

	unsigned set = extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1);
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	if (sets[set].add2Set(tag, writing_policy_, L1_or_L2_, tag_to_evict))
	{
		return true;
	}
	else 
	{
		return false;
	}
}




void cache::removeTag(unsigned address, unsigned tag_to_evict)
{
	numOfAccess_++;

	unsigned set = extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1);
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	sets[set].removetag(tag);
}





/* Name: ReadCache
   Descriptioin: reads from the cache updates the number of access to the cache
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
*/
bool cache::ReadCache(unsigned address){
	numOfAccess_++;
	unsigned set = extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1);
	// ASDF(offset_bits_ + set_bits_ - 1)
	// ASDF(offset_bits_)
	// ASDF(set)
	if (sets[set].readSet(set)) return true;
	miss_++;
	ASDF("here1")
	return false;
}


/* Name: getnumOfAccess
   Descriptioin: returns number of access to the cache
   parameters: None
   return value: numOfAccess
*/
double cache::getnumOfAccess()const
{
	return numOfAccess_;
}

bool cache::get_WR() const{
	return writing_policy_;
}


bool cache::get_L1_L2() const{
	return L1_or_L2_;
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

/* Name: updateTime
Descriptioin: updates the time of access to the cache
parameters: None
return value: None
*/
void cache::updateTime()
{
	time_ += cache_cycle_;
}
