#pragma once
#ifndef Sets.h
#include <list>
#include <vector>

typedef struct map_
{
	int dirty; // 1 valid 0 not valid
	uint32_t tag;
}Map;

class Cache_set
{
public:
	Cache_set(uint32_t ways);
	bool write2Set(uint32_t tag);
	bool readSet(uint32_t tag);
	void updateLRU(uint32_t MRU);  
	uint32_t lru_from_LRU();
	uint32_t find_tag(uint32_t tag);
	~Cache_set();

private:
	std::list<Map> ways_;
	std::vector<uint32_t> LRU_arr_;
};

#endif // !Sets.h
