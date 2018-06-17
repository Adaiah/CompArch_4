#pragma once

#ifndef Cache_h
#include <iostream>
#include <vector>
#include <map>
#include "Sets.h"



class cache
{
public:
	cache(unsigned Csize, unsigned cache_cyc, unsigned Bsize, unsigned Assoc, bool WrAlloc, bool L1_or_L2);
	~cache();
	unsigned get_miss() const;
	bool Write2Cache(unsigned address, bool real_write, bool dirty);
	bool Add2Cache(unsigned address, unsigned* tag_to_evict);
	void removeTag(unsigned tag_to_evict);
	void clear(unsigned address, cache& other);
	bool ReadCache(unsigned address);
	double getnumOfAccess()const;
	void updateTime();
	int gettime()const;
	bool get_WR()const;
	bool get_L1_L2()const;

	
private:
	unsigned offset_bits_;
	unsigned set_bits_;
	unsigned tag_bits_;
	unsigned cache_size_;
	unsigned cache_cycle_;
	unsigned Bsize_;
	unsigned Assoc_;
	unsigned sets_;
	int miss_;
	bool writing_policy_;
	int time_;
	double numOfAccess_;
	bool L1_or_L2_;
	std::vector<Cache_set> sets;
};

#endif // !Cache.h
