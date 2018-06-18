#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "Sets.h"

using std::FILE;
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::stringstream;



int main(int argc, char **argv) {
	if (argc < 19) {
		cerr << "Not enough arguments" << endl;
		return 0;
	}

	// Get input arguments

	// File
	// Assuming it is the first argument
	char* fileString = argv[1];
	ifstream file(fileString); //input file stream
	string line;
	if (!file || !file.good()) {
		// File doesn't exist or some other error
		cerr << "File not found" << endl;
		return 0;
	}

	unsigned MemCyc = 0, BSize = 0, L1Size = 0, L2Size = 0, L1Assoc = 0,
			L2Assoc = 0, L1Cyc = 0, L2Cyc = 0, WrAlloc = 0;

	for (int i = 2; i < 19; i += 2) {
		string s(argv[i]);
		if (s == "--mem-cyc") {
			MemCyc = atoi(argv[i + 1]);
		} else if (s == "--bsize") {
			BSize = atoi(argv[i + 1]);
		} else if (s == "--l1-size") {
			L1Size = atoi(argv[i + 1]);
		} else if (s == "--l2-size") {
			L2Size = atoi(argv[i + 1]);
		} else if (s == "--l1-cyc") {
			L1Cyc = atoi(argv[i + 1]); 
		} else if (s == "--l2-cyc") {
			L2Cyc = atoi(argv[i + 1]);
		} else if (s == "--l1-assoc") {
			L1Assoc = atoi(argv[i + 1]);
		} else if (s == "--l2-assoc") {
			L2Assoc = atoi(argv[i + 1]);
		} else if (s == "--wr-alloc") {
			WrAlloc = atoi(argv[i + 1]);
		} else {
			cerr << "Error in arguments" << endl;
			return 0;
		}
	}
	cache L1(L1Size, L1Cyc, BSize, L1Assoc, WrAlloc, L1T);
	cache L2(L2Size, L2Cyc, BSize, L2Assoc, WrAlloc, L2T);
	unsigned time_mem = 0; // if misses in both L1 and L2 then update by adding MemCyc
	unsigned mem_access = 0; //if accessed mem advance by 1;
	while (getline(file, line)) {

		stringstream ss(line);
		string address;
		char operation = 0; // read (R) or write (W)
		if (!(ss >> operation >> address)) {
			// Operation appears in an Invalid format
			cout << "Command Format error" << endl;
			return 0;
		}

		// DEBUG - remove this line
		// cout << "operation: " << operation;

		string cutAddress = address.substr(2); // Removing the "0x" part of the address

		// DEBUG - remove this line  
		// cout << ", address (hex)" << cutAddress;

		unsigned num = 0;
		num = strtoul(cutAddress.c_str(), NULL, 16);

		// DEBUG - remove this line
		// cout << " (dec) " << num << endl;
		if(operation=='r')
		{
			L1.updateTime();
			if(!L1.ReadCache(num))
			{
				L1.clear(num, L2); //if needed. L2 as argument to drop to
				L2.updateTime();
				if(!L2.ReadCache(num)){// if miss in both L1 L2 calc time using mem
					int tmp = L2.clear(num, L1);
					time_mem+=MemCyc * tmp; //if needed. L1 as argument to drop to
					mem_access+= tmp;
					
					L2.Write2Cache(num, false, false);// if missed in both cache than write update writing to both cache
				}
				//if miss only in L1 write to L1 and update time checking writing to L1 and readinf L2
				L1.Write2Cache(num, false, false);
			}
		}

		if(operation=='w')
		{
			L1.updateTime();
			if(!L1.Write2Cache(num, true, true))
			{
				if (WrAlloc)
				{
					//clear space in L1, drop to L2 if needed
					L1.clear(num, L2); //if needed. L2 as argument to drop to
					L2.updateTime();
					if (!L2.Write2Cache(num, true, true))//not in L2
					{
						int tmp =L2.clear(num, L1);
						time_mem+=MemCyc * tmp;//if needed. L1 as argument to drop to
						mem_access+= tmp;
						//add to L2!!!!!!!!!!!!!!!
						L2.Write2Cache(num, false, false);//bring from memory

					}
					L1.Write2Cache(num, false, true);
					//add to L1!!!!!!!!!!!

				}
				else//no WrAlloc
				{
					ASDF("no alloc???????")
					L2.updateTime();
					if (!L2.Write2Cache(num, true, true))//not in L2
					{
						time_mem+=MemCyc;
						mem_access++;
					}
				}
			}
		}
	}

	double L1MissRate;
	double L2MissRate;
	double avgAccTime;
	L1MissRate = L1.get_miss() / L1.getnumOfAccess();
	L2MissRate = L2.get_miss() / L2.getnumOfAccess();
	avgAccTime = (L1.gettime() + L2.gettime() + time_mem) / (L1.getnumOfAccess() + L2.getnumOfAccess() + mem_access);//maybe without mem_access

	printf("L1miss=%.03f ", L1MissRate);
	printf("L2miss=%.03f ", L2MissRate);
	printf("AccTimeAvg=%.03f\n", avgAccTime);

	return 0;
}
