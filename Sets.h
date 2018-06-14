#pragma once
#ifndef Sets.h
#include <list>
#include <vector>
typedef struct map_
{
	int dirty; // 1 valid 0 not valid
	unsigned int tag;
}Map;

class Cache_set
{
public:
	Cache_set(unsigned ways);
	bool write2Set(unsigned tag);
	bool readSet(unsigned tag);
	void updateLRU(unsigned MRU);  
	int find_tag(unsigned tag);
	~Cache_set();

private:
	std::list<Map> ways_;
	std::vector<unsigned int> LRU_arr_;
};

#endif // !Sets.h
