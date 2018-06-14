#pragma once

#ifndef Cache.h
#include <iostream>
#include <vector>
#include "Sets.h"

class cache
{
public:
	cache(unsigned Csize, unsigned cache_cyc, unsigned Bsize, unsigned Assoc, unsigned WrAlloc);
	~cache();
	unsigned get_miss() const;
	bool Write2Cache(unsigned address);
	bool ReadCache(unsigned address);
	void updateTime();// Added!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	double getnumOfAccess()const;
	int gettime()const;
	unsigned calc_set(unsigned address);
	unsigned calc_tag(unsigned address);

	
private:
	unsigned cache_size_;
	unsigned cache_cycle_;
	unsigned Bsize_;
	unsigned Assoc_;
	int miss_;
	unsigned writing_policy_;
	int time_;
	double numOfAccess_;
	std::vector<Cache_set> sets;
};

#endif // !Cache.h
