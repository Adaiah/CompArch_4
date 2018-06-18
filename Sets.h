#pragma once
#ifndef Sets_h
#include <list>
#include <vector>
#include <iostream>
using std::cout;
using std::endl;

#define L1T 0
#define L2T 1

#define ASDF(x) cout << "debug: "<< x << endl;

typedef struct map_
{
	bool dirty;
	unsigned tag;
	bool valid;
} Map;

class Cache_set
{
public:
	Cache_set(unsigned ways);
	bool write2Set(unsigned tag, unsigned address, bool real_write, bool dirty, bool L1_or_L2);
	bool clearSet(unsigned address,unsigned tag, bool L1_or_L2, unsigned* tag_to_evict);
	bool removetag(unsigned tag);
	bool readSet(unsigned tag);
	void updateLRU(unsigned MRU);  
	unsigned lru_from_LRU();
	unsigned find_tag(unsigned tag);
	~Cache_set();

private:
	std::list<Map> ways_;
	std::list<unsigned> LRU_arr_;
};

#endif // !Sets.h
