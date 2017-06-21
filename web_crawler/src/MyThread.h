/* MyThread.h
* CSCE 613 - 600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

#ifndef   _my_thread_h_   
#define   _my_thread_h_   

#include "common.h"
#include "MySocket.h"
#include "MySet.h"

#define N_LINE_DEQUEUE 1
#define STAT_PRINT_INTERVAL 2 // unit s
#define REPORT 0

class HttpStatus {
public:
	volatile long two_h; // 2xx
	volatile long three_h; // 3xx
	volatile long four_h; // 4xx
	volatile long five_h; // 5xx
	volatile long other; // others

	HttpStatus()
	{
		two_h = 0; three_h = 0; four_h = 0; five_h = 0; other = 0;
	}
};

class Statistic {
public:
	int tCount;
	volatile long Q; // current size of pending queue
	volatile long E; // number of extracted URLs from the queue
	volatile long H; // number of URLs that have passed host uniqueness
	volatile long D; // number of successful DNS lookups
	volatile long I; // number of URLs that have passed IP uniqueness
	volatile long R; // number of URLs that have passed robots checks
	volatile long C; // number of successfully crawled URLs(those with a valid HTTP code)
	volatile long L; // total links found
	volatile long B; // page bytes in total

	volatile long R_PB;
	volatile long R_numTAMU;
	volatile long R_numOTAMU;
	HttpStatus HttpCode;
	Statistic()
	{
		Q = 0; E = 0; H = 0; D = 0; I = 0; R = 0; C = 0; L = 0; B = 0;

		R_PB = 0;
		R_numTAMU = 0;
		R_numOTAMU = 0;
	}
};

class Parameters {
public:
	InputType InType;
	CRITICAL_SECTION URLQueueCS;
	CRITICAL_SECTION seenHostCS;
	CRITICAL_SECTION seenIPsCS;
	HANDLE eventQuit;
	char *QueueBuffer;
	HostSet *seenHosts;
	IPSet *seenIPs;

	Parameters(InputType in, char *buf, HostSet *HSet, IPSet *IPSet)
	{
		InType = in;
		InitializeCriticalSection(&URLQueueCS);
		InitializeCriticalSection(&seenHostCS);
		InitializeCriticalSection(&seenIPsCS);
		eventQuit = CreateEvent(NULL, true, false, NULL);
		QueueBuffer = buf;
		seenHosts = HSet;
		seenIPs = IPSet;
	}

	~Parameters()
	{
		DeleteCriticalSection(&URLQueueCS);
		DeleteCriticalSection(&seenHostCS);
		DeleteCriticalSection(&seenIPsCS);
		CloseHandle(eventQuit);
	}
};

UINT ThreadStat(LPVOID pParam);
UINT ThreadCrawling(LPVOID pParam);

#endif 