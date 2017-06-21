/* MyThread.cpp
* CSCE 613 - 600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

#include "MyThread.h"
#include "StepFunction.h"
#include "MySet.h"

extern Statistic Stats;

UINT ThreadStat(LPVOID pParam)
{
	Parameters *p = ((Parameters*)pParam);
	double pps = 0;
	double Mbps = 0;

	while (WaitForSingleObject(p->eventQuit, STAT_PRINT_INTERVAL*1000) == WAIT_TIMEOUT)
	{
		Stats.tCount++;
		if (Stats.tCount != 0)
		{
			pps = double(Stats.R) / (Stats.tCount * STAT_PRINT_INTERVAL);
			Mbps = double(Stats.B) / 128 / 1024 / (Stats.tCount * STAT_PRINT_INTERVAL); // bytes to bits
		}
		
		// print 
		//system("cls"); // clear console
		printf("[%3d] %6d Q %7d E %6d H %6d D %5d I %5d R %5d C %4d L \n", Stats.tCount*STAT_PRINT_INTERVAL, Stats.Q, Stats.E, Stats.H, Stats.D, Stats.I, Stats.R, Stats.C, Stats.L);
		printf("\t*** crawling @%.1lf pps @%.1lf Mbps \n", pps, Mbps);
	}

	printf("\n");
	printf("Extracted %d URLs @ %d/s \n", Stats.E, Stats.E / (Stats.tCount * STAT_PRINT_INTERVAL));
	printf("Looked up %d DNS names @ %d/s \n", Stats.D, Stats.D / (Stats.tCount * STAT_PRINT_INTERVAL));
	printf("Downloaded %d robots @ %d/s \n", Stats.I, Stats.I / (Stats.tCount *STAT_PRINT_INTERVAL));
	printf("Crawled %d pages @ %d/s (%.2lf MB) \n", Stats.R, Stats.R / (Stats.tCount * STAT_PRINT_INTERVAL), (double)(Stats.B/1024/1024));
	printf("Parsed %d links @ %d/s \n", Stats.L, Stats.L / (Stats.tCount * STAT_PRINT_INTERVAL));
	printf("HTTP codes: 2xx = %d, 3xx = %d, 4xx = %d, 5xx = %d, other = %d \n",Stats.HttpCode.two_h, Stats.HttpCode.three_h, Stats.HttpCode.four_h, Stats.HttpCode.five_h, Stats.HttpCode.other);

	if (REPORT)
	{
		printf("page size in bytes (across all HTTP codes) = %d\n", Stats.R_PB);
		printf("pages contain hyperlink of tamu.edu = %d\n", Stats.R_numTAMU);
		printf("pages contain hyperlink of tamu.edu outside TAMU = %d\n", Stats.R_numOTAMU);
	}

	return 0;
}


UINT ThreadCrawling(LPVOID pParam)
{
	Parameters *p = ((Parameters*)pParam);

	// Initialize Socket class
	Socket ClientSocket;
	// check buffer and socket allocated/opened successfuuly
	if (ClientSocket.SelfCheck() == false)
	{
		printf("Thread %d ClientSocket initialization failed \n", GetCurrentThreadId());
		return 0;
	}

	InterlockedIncrement(&Stats.Q);

	char StrHost[MAX_HOST_LEN] = { 0 };
	int Port;
	char StrRequest[MAX_REQUEST_LEN] = { 0 };
	char *url = NULL;
	char *NewLineStart = NULL;
	char *NewnLineStart = NULL;
	int nLinks;
	int RnBytes;
	int PnBytes;
	int HttpCode;

	// create new parser object
	HTMLParserBase *parser = new HTMLParserBase;

	while (true)
	{
		// enter critical section, access the URL queue
		EnterCriticalSection(&p->URLQueueCS); // enter critical section

		NewLineStart = p->QueueBuffer;
		if ((NewnLineStart = FindEndOfnLine(NewLineStart, N_LINE_DEQUEUE)) == NULL)
		{
			LeaveCriticalSection(&p->URLQueueCS); // leave critical section
			InterlockedDecrement(&Stats.Q);
			delete parser;		// this internally deletes linkBuffer
			return 0;
		}
		p->QueueBuffer = NewnLineStart;
		LeaveCriticalSection(&p->URLQueueCS); // leave critical section
		
		while (true)
		{
			url = NewLineStart;
			if ((NewLineStart = FindEndOfLine(NewLineStart)) == NULL)
				break;
			if (p->InType != I_MT_FILE)
				printf("URL: %s\n", url);
			InterlockedIncrement(&Stats.E);
			/* STEP1 URL parses */
			if (ParsingURL(url, StrHost, &Port, StrRequest, p->InType) == false)
				continue;

			/* STEP2 Check Hosts uniqueness */
			if (p->InType != I_URL)
			{
				std::string str(StrHost);
				// enter critical section, check host sets
				EnterCriticalSection(&p->seenHostCS); // enter critical section
				if (CheckHostUnique(p->seenHosts, str, p->InType) == false)
				{
					LeaveCriticalSection(&p->seenHostCS); // leave critical section
					continue;
				}
				LeaveCriticalSection(&p->seenHostCS); // leave critical section
				InterlockedIncrement(&Stats.H);
			}

			/* STEP3 DNS resolve */
			IN_ADDR IP_address;

			if (DoingDNS(StrHost, &IP_address, p->InType) == false)
			{
				continue;
			}
			InterlockedIncrement(&Stats.D);

			/* STEP4 Check IPs uniqueness */
			if (p->InType != I_URL)
			{
				DWORD ip = inet_addr(inet_ntoa(IP_address));
				// enter critical section, check host sets
				EnterCriticalSection(&p->seenIPsCS); // enter critical section
				if (CheckIPUnique(p->seenIPs, ip, p->InType) == false)
				{
					LeaveCriticalSection(&p->seenIPsCS); // leave critical section
					continue;
				}
				LeaveCriticalSection(&p->seenIPsCS); // leave critical section
				InterlockedIncrement(&Stats.I);
			}

			// Initialize IP and port #
			ClientSocket.Initialize(&IP_address, Port);

			/* STEP5 Request robots */
			if (p->InType != I_URL)
			{
				if (SendHttpRequest(&ClientSocket, RequestTarget(ROBOT), ValidStatus(FOUR), StrHost, StrRequest, 16 * 1024, p->InType, &RnBytes, &HttpCode) == false)
				{
					InterlockedAdd(&Stats.B, (LONG)RnBytes);
					continue;
				}
				InterlockedAdd(&Stats.B, (LONG)RnBytes);
				InterlockedIncrement(&Stats.R);
			}

			/* STEP6 Request page */
			if (SendHttpRequest(&ClientSocket, RequestTarget(PAGE), ValidStatus(TWO), StrHost, StrRequest, 2048 * 1024, p->InType, &PnBytes, &HttpCode) == false)
			{
				InterlockedAdd(&Stats.B, (LONG)PnBytes);
				InterlockedAdd(&Stats.R_PB, (LONG)RnBytes);
				if (HttpCode != 0)
				{
					if (HttpCode >= 300 && HttpCode < 400)
						InterlockedIncrement(&Stats.HttpCode.three_h);
					else if (HttpCode >= 400 && HttpCode < 500)
						InterlockedIncrement(&Stats.HttpCode.four_h);
					else if (HttpCode >= 500 && HttpCode < 600)
						InterlockedIncrement(&Stats.HttpCode.five_h);
					else
						InterlockedIncrement(&Stats.HttpCode.other);
				}
				continue;
			}
			InterlockedIncrement(&Stats.C);
			InterlockedIncrement(&Stats.HttpCode.two_h);
			InterlockedAdd(&Stats.B, (LONG)PnBytes);
			InterlockedAdd(&Stats.R_PB, (LONG)RnBytes);

			bool linkTAMU = false;
			bool OutsideTAMU = true;
			/* STEP7 Parsing page */
			if (ParsingPage(parser, &ClientSocket, url, p->InType, &nLinks, &linkTAMU, &OutsideTAMU) == false)
			{
				// only error when not enough memory
				continue;
			}
			InterlockedAdd(&Stats.L, (LONG)nLinks);
			if (linkTAMU)
			{
				InterlockedIncrement(&Stats.R_numTAMU);
				if (OutsideTAMU)
					InterlockedIncrement(&Stats.R_numOTAMU);
			}
			
		}
	}

}
