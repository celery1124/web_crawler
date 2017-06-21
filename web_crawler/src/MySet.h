/* MySet.h
* CSCE 613 - 600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

#ifndef   _my_set_h_   
#define   _my_set_h_   

#include "common.h"

class HostSet
{
public:
	unordered_set<string> Hosts;
	int prevSize;

	HostSet(void)
	{
		prevSize = 0;
	}
};

class IPSet
{
public:
	unordered_set<DWORD> IPs;
	int prevSize;

	IPSet(void)
	{
		prevSize = 0;
	}
};

#endif