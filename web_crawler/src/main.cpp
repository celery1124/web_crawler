/* main.cpp
* CSCE 613-600 Spring 2017 
* HW1 web_crawler
* by Mian Qin
*/

#include "stdafx.h"
#include "MySocket.h"
#include "MySet.h"
#include "StepFunction.h"
#include "MyThread.h"
//#include "vld.h"

Statistic Stats;

void print_usage() {
	printf("usage : web_crawler.exe number_of_thread URL_filename\n");
	printf("   or : web_crawler.exe url\n");
	printf("url format : scheme://host[:port][/path][?query][#fragment]\n");
	printf("Example : web_crawler.exe 1 URL-input.txt\n");
	printf("     or : web_crawler.exe celery1124.com\n");
}

bool InitialWinsock()
{
	WSADATA wsaData;
	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	return true;
}

void CleanWinsock()
{
	WSACleanup();
}

int main(int argc, char* argv[])
{
	int ret;
	InputType InType;
	char *filename = NULL;
	char *fileBuf = NULL;
	int nThread = 1;
	//clock_t start_t, end_t; // performance used only

	/* check input argument numbers */
	if (argc > 3) {
		printf("wrong number of argument \n");
		print_usage();
		return 0;
	}
	// 1 argument format
	else if (argc == 2) 
	{
		InType = I_URL;
		fileBuf = argv[1];
	}
	// 2 argument format
	else {
		nThread = atoi(argv[1]);
			if (nThread == 1)
			{
				InType = I_FILE;
			}
			else
			{
				InType = I_MT_FILE;
			}
		filename = argv[2];
		/* Open input file */
		if ((ret = OpenFile(filename, &fileBuf)) == false)
		{
			return 0;
		}
	}

	/* initial winsock */
	if ((ret = InitialWinsock()) == false)
		return 0;

	/* Initialize seenHost and seenIP set */
	HostSet seenHosts;
	IPSet seenIPs;
	Parameters p(InType, fileBuf, &seenHosts, &seenIPs);

	HANDLE handles_stats;
	HANDLE *handles_crawler = new HANDLE[nThread];

	//start_t = clock();

	if (InType == I_MT_FILE)
	{
		/* start stats thread */
		handles_stats = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadStat, &p, 0, NULL);
	}

	/* start N crawling threads */
	for (int i = 0; i < nThread; i++)
	{
		handles_crawler[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCrawling, &p, 0, NULL);
	}
	
	/* wait for N crawling threads to finish */
	for (int i = 0; i < nThread; i++)
	{
		WaitForSingleObject(handles_crawler[i], INFINITE);
		CloseHandle(handles_crawler[i]);
	}

	if (InType == I_MT_FILE)
	{
		/* notify stats thread to finish */
		SetEvent(p.eventQuit);
		WaitForSingleObject(handles_stats, INFINITE);
		CloseHandle(handles_stats);
	}
	

	// free the url buffer read from file
	free(fileBuf);
	if (InType == I_MT_FILE)
		delete handles_crawler;

	//end_t = clock();

	/* Cleanning Winsock */
	CleanWinsock();

	//printf("\ntime elapsed:%d ms\n", (int)(end_t - start_t));

	return 0;
}
