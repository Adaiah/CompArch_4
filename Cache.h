#pragma once

#ifndef Cache.h
#include <iostream>
#include <vector>
#include <map>
#include "Sets.h"

class cache
{
public:
	cache(uint32_t Csize, uint32_t cache_cyc, uint32_t Bsize, uint32_t Assoc, char WrAlloc);
	~cache();
	uint32_t get_miss() const;
	bool Write2Cache(uint32_t address);
	bool ReadCache(uint32_t address);
	double getnumOfAccess()const;
	int gettime()const;
	uint32_t calc_set(uint32_t address);
	uint32_t calc_tag(uint32_t address);

	
private:
	uint32_t offset_bits_;
	uint32_t set_bits_;
	uint32_t tag_bits_;
	uint32_t cache_size_;
	uint32_t cache_cycle_;
	uint32_t Bsize_;
	uint32_t Assoc_;
	uint32_t sets_;
	int miss_;
	bool writing_policy_;
	int time_;
	double numOfAccess_;
	std::vector<Cache_set> sets;
};

#endif // !Cache.h
