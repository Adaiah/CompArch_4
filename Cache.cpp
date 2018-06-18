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
	if (to + 1 != CMDSIZE)
	{
		num = num % powerOf2(to + 1);
	}
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

cache::~cache(){}

//i- evicted times from L2 to memory
//only L2 will change it
int cache::clear(unsigned address, cache& other)
{
	int evicted = 1;
	unsigned set = (set_bits_) ? extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1) : 0;
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	unsigned tag_to_evict;
	if (!sets[set].clearSet(address, tag, L1_or_L2_, &tag_to_evict))
	{
		unsigned num = tag_to_evict * powerOf2(offset_bits_ + set_bits_) + set * powerOf2(offset_bits_);
		if (L1_or_L2_ == L2T)// other is L1
		{
			other.updateTime();
			if (other.removeTag(num))
			{
				// it was dirty in L1
				evicted += 1;
			}
		}
		else if (L1_or_L2_ == L1T)
		{
			//if is dirty change in L2
			//considered access
			// evicted += other.clear(num, *this);
			other.updateTime();
			other.Write2Cache(num, false, true);
		}
	}
	return evicted;
}


bool cache::removeTag(unsigned address)
{
	unsigned set = (set_bits_) ? extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1) : 0;
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	return sets[set].removetag(tag);
}



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
bool cache::Write2Cache(unsigned address, bool real_write, bool dirty)
{
	if(real_write) numOfAccess_++;
	unsigned set = (set_bits_) ? extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1) : 0;
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	if (sets[set].write2Set(tag, address, real_write, dirty, L1_or_L2_))
	{
		return true;
	}
	else
	{
		if(real_write) miss_++;
		return false;
	}
}

/* Name: ReadCache
   Descriptioin: reads from the cache updates the number of access to the cache
   parameters: address- address of the data. will be used to calculate
					tag, offset, set
*/
bool cache::ReadCache(unsigned address){
	numOfAccess_++;
	unsigned set = (set_bits_) ? extractBits(address, offset_bits_, offset_bits_ + set_bits_ - 1) : 0;
	unsigned tag = extractBits(address, offset_bits_ + set_bits_, CMDSIZE - 1);
	if (sets[set].readSet(tag))
	{
		return true;
	}
	else
	{
		miss_++;
		return false;
	}
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
