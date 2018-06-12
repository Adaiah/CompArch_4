#pragma once
#ifndef Sets.h
#include <list>
#define OFFSET 4
typedef struct map_
{
	int dirty;
	unsigned int tag;
}Map;

class Cache_set
{
public:
	Cache_set(unsigned ways);
	unsigned int write2Set(unsigned tag);
	unsigned int readSet(unsigned tag);
	void updateLRU(unsigned MRU);  
	int find_tag(unsigned tag);
	~Cache_set();

private:
	std::list<Map> ways_;
};

#endif // !Sets.h
