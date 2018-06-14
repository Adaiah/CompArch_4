#include "Cache.h"
#define CMDSIZE 32

/*calculates the power of 2 according to the exponent given*/
static uint32_t powerOf2(uint32_t i)
{
	uint32_t result = 1;
	for (uint32_t j=0; j < i; j++)
	{
		result *= 2;
	}
	return result;
}

static uint32_t extractBits(uint32_t num, uint32_t from, uint32_t to) { // include from and to
	num %= (to + 1);
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
cache::cache(uint32_t Csize, uint32_t cache_cyc, uint32_t Bsize, uint32_t Assoc, bool WrAlloc) {
	miss_ = 0;
	time_ = 0;
	numOfAccess_ = 0;
	writing_policy = WrAlloc;
	offset_bits_ = Bsize;
	set_bits_ = Csize - Bsize - Assoc;
	tag_bits_ = CMDSIZE - offset_bits_ - set_bits_;
	cache_cycle_ = cache_cyc;
	Bsize_ = powerOf2(Bsize);
	cache_size_ = powerOf2(Csize);
	Assoc_ = powerOf2(Assoc);
	sets_ = powerOf2(set_bits_);//maybe we dont have enough space to use all of the power2(set_bits_)????

	for (int i=0; i<sets_;i++)
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
uint32_t cache::get_miss() const{
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
bool cache::Write2Cache(uint32_t address)
{
	numOfAccess_++;
	uint32_t offset = extractBits(address, 0, offset_bits_ - 1);
	uint32_t set = calc_set(address);
	uint32_t tag = calc_tag(address);
	if (sets[set].readSet(tag)) return true;
	//miss


}

/* Name: ReadCache
   Descriptioin: reads from the cache
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
*/
bool cache::ReadCache(uint32_t address){
	
	
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
   parameters: address
   return value: set
*/
uint32_t cache::calc_set(uint32_t address){
	return extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1);

}

/* Name: calc_tag
   Descriptioin: calculates tag of a certain address for writing/reading
   parameters: address
   return value: tag
*/
uint32_t cache::calc_tag(uint32_t address)
{
	return extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
}




