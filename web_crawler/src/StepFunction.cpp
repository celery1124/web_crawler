/* StepFunction.cpp
* CSCE 613-600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

#include "StepFunction.h"
#include "MyThread.h"

bool OpenFile(char *filename, char **fileBuf)
{
	// open file
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	// process errors
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Input File error: CreateFile failed with %d\n", GetLastError());
		return false;
	}
	// get file size
	LARGE_INTEGER FS;
	// process errors
	if (GetFileSizeEx(hFile, &FS) == 0)
	{
		printf("Input File error: GetFileSizeEx error %d\n", GetLastError());
		return false;
	}

	// read file into a buffer
	int fileSize = (DWORD)FS.QuadPart;			// assumes file size is below 2GB; otherwise, an __int64 is needed
	DWORD bytesRead;
	// allocate buffer
	*fileBuf = (char*)malloc(fileSize + 1); // treat it as string to compatible with URL input
	// also guarantee FindEndOfLine works since it use strstr();
	if (*fileBuf == NULL)
	{
		printf("failed with malloc error out of memory \n");
		return false;
	}
	// read into the buffer
	if (ReadFile(hFile, *fileBuf, fileSize, &bytesRead, NULL) == 0 || bytesRead != fileSize)
	{
		printf("Input File error: ReadFile failed with %d\n", GetLastError());
		return false;
	}
	*(*fileBuf+fileSize) = '\0';

	// close file handle
	CloseHandle(hFile);
	printf("Opened %s with size %d\n", filename, bytesRead);
	return true;
}

bool ParsingURL(char *Inputurl, char *StrHost, int *Port, char *StrRequest, InputType type)
{
	if (type != I_MT_FILE)
		printf("\tParsing URL... ");
	char *url = NULL;

	// check scheme
	if (strncmp(Inputurl, "http://", 7) != 0) {
		if (type != I_MT_FILE)
			printf("failed with invalid scheme \n");
		return false;
	}
	else {
		if ((url = (char*)malloc(strlen(Inputurl) - 6)) == NULL)
		{
			printf("failed with malloc error out of memory \n");
			return false;
		}
		memcpy(url, Inputurl + 7, strlen(Inputurl) - 6); // remove "http://"
	}

	int HostLen = 0;
	int RequestLen = 0;
	char *p_mark = NULL;
	char *StrPath = NULL;
	char *StrQuery = NULL;
	char *StrFragment = NULL; // Maybe for future use
	
	*Port = 80; //default port number
	// get fragment
	if ((p_mark = strchr(url, '#')) != NULL)
	{
		if ((StrFragment = (char*)malloc(strlen(p_mark) + 1)) == NULL)
		{
			printf("failed with malloc error out of memory \n");
			return false;
		}
		memcpy(StrFragment, p_mark, strlen(p_mark) + 1);
		*p_mark = '\0';
	}
	else
		StrFragment = url + strlen(url); // ensure strlen(StrFragment) = 0;
	// get query
	if ((p_mark = strchr(url, '?')) != NULL)
	{
		if ((StrQuery = (char*)malloc(strlen(p_mark) + 1)) == NULL)
		{
			printf("failed with malloc error out of memory \n");
			return false;
		}
		memcpy(StrQuery, p_mark, strlen(p_mark) + 1);
		*p_mark = '\0';
	}
	else
		StrQuery = url + strlen(url); // ensure strlen(StrQuery) = 0;
	// get path
	if ((p_mark = strchr(url, '/')) != NULL)
	{
		if ((StrPath = (char*)malloc(strlen(p_mark) + 1)) == NULL)
		{
			printf("failed with malloc error out of memory \n");
			return false;
		}
		memcpy(StrPath, p_mark, strlen(p_mark) + 1);
		*p_mark = '\0';
	}
	else // add path /
	{
		if ((StrPath = (char*)malloc(2)) == NULL)
		{
			printf("failed with malloc error out of memory \n");
			return false;
		}
		strncpy(StrPath, "/", 2);
	}
	// get port
	if ((p_mark = strchr(url, ':')) != NULL)
	{
		//get port
		*Port = atoi(p_mark + 1);
		if (*Port <= 0 || *Port >= 65535) {
			printf("failed with invalid port \n");
			return false;
		}
		*p_mark = '\0';
	}
	HostLen = strlen(url);
	RequestLen = strlen(StrPath) + strlen(StrQuery) ;

	if (HostLen > MAX_HOST_LEN - 1)
	{
		// free malloc space in heap
		if (strlen(StrPath))
			free(StrPath);
		if (strlen(StrQuery))
			free(StrQuery);
		if (strlen(StrFragment))
			free(StrFragment);
		free(url);
		if (type != I_MT_FILE)
			printf("Beyond Maximun host length\n");
		return false;
	}
	if (RequestLen > MAX_REQUEST_LEN - 1)
	{
		// free malloc space in heap
		if (strlen(StrPath))
			free(StrPath);
		if (strlen(StrQuery))
			free(StrQuery);
		if (strlen(StrFragment))
			free(StrFragment);
		free(url);
		if (type != I_MT_FILE)
			printf("Beyond Maximun request length\n");
		return false;
	}

	memcpy(StrHost, url, HostLen);
	*(StrHost + HostLen) = '\0';
	sprintf(StrRequest, "%s%s", StrPath, StrQuery);
	*(StrRequest + RequestLen) = '\0';

	// free malloc space in heap
	if (strlen(StrPath))
		free(StrPath);
	if (strlen(StrQuery))
		free(StrQuery);
	if (strlen(StrFragment))
		free(StrFragment);
	free(url);

	switch (type)
	{
	case I_URL:
		printf("host %s, port %d, request %s\n", StrHost, *Port, StrRequest);
		break;
	case I_FILE:
		printf("host %s, port %d\n", StrHost, *Port);
		break;
	case I_MT_FILE:
		break;
	default:
		printf("\tfalse target argument");
		return false;
	}
	
	return true;
}

bool CheckHostUnique(HostSet *seenHosts, string test, InputType type)
{
	if (type != I_MT_FILE)
		printf("\tChecking host uniqueness... ");
	(*seenHosts).Hosts.insert(test);
	if ((*seenHosts).Hosts.size() > (*seenHosts).prevSize)
	{ // unique host
		(*seenHosts).prevSize++;
		if (type != I_MT_FILE)
			printf("passed\n");
		return true;
	}
	else
	{ // duplicate host
		if (type != I_MT_FILE)
			printf("failed\n");
		return false;
	}
}

bool CheckIPUnique(IPSet *seenIPs, DWORD test, InputType type)
{
	if (type != I_MT_FILE)
		printf("\tChecking IP uniqueness... ");
	(*seenIPs).IPs.insert(test);
	if ((*seenIPs).IPs.size() > (*seenIPs).prevSize)
	{ // unique IP
		(*seenIPs).prevSize++;
		if (type != I_MT_FILE)
			printf("passed\n");
		return true;
	}
	else
	{ // duplicate IP
		if (type != I_MT_FILE)
			printf("failed\n");
		return false;
	}
}

bool DoingDNS(char *StrHost, IN_ADDR *IP_address, InputType type)
{
	clock_t t_start, t_finish;

	if (type != I_MT_FILE)
		printf("\tDoing DNS... ");
	t_start = clock();

	// structure used in DNS lookups
	struct hostent *hostname;

	DWORD IP = inet_addr(StrHost);
	if (IP == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		if ((hostname = gethostbyname(StrHost)) == NULL)
		{
			if (type != I_MT_FILE)
				printf("failed with %d \n", WSAGetLastError());
			return false;
		}
		else // take the first IP address and copy into sin_addr
			memcpy((char *)(IP_address), hostname->h_addr, hostname->h_length);
	}
	else
	{
		// if a valid IP, directly drop its binary version into sin_addr
		(*IP_address).S_un.S_addr = IP;
	}
	t_finish = clock();

	if (type != I_MT_FILE)
		printf("done in %d ms, found %s \n", int(t_finish - t_start), inet_ntoa(*IP_address));
	return true;
}

bool ConnectingOnPage(Socket *ClientSocket, RequestTarget target, InputType type)
{
	clock_t t_start, t_finish;
	switch (target)
	{
	case ROBOT :
		if (type != I_MT_FILE)
			printf("\tConnecting on robot... ");
		break;
	case PAGE :
		if (type != I_MT_FILE)
			printf("      * Connecting on page... ");
		break;
	default:
		if (type != I_MT_FILE)
			printf("\tflase target argument");
		return false;
	}
	
	t_start = clock();
	if ((*ClientSocket).Connect(type) == false)
	{
		return false;
	}

	t_finish = clock();
	if (type != I_MT_FILE)
		printf("done in %d ms\n", int(t_finish - t_start));
	return true;
}

bool Loading(Socket *ClientSocket, const char* HTTPrequest, char *StrHost, char *StrRequest, int MaxDLSize, InputType type, int *NBytes)
{
	clock_t t_start, t_finish;
	char *sendbuf;
	int sendbuf_size;
	if (type != I_MT_FILE)
		printf("\tLoading... ");

	*NBytes = 0;
	t_start = clock();
	sendbuf_size = strlen(HTTPrequest) + strlen(StrHost) + strlen(StrRequest) - 3;
	if ((sendbuf = (char*)malloc(sendbuf_size)) == NULL)
	{
		printf("failed with malloc error out of memory \n");
		return false;
	}
	sprintf(sendbuf, HTTPrequest, StrRequest, StrHost);
	sendbuf[sendbuf_size-1] = '\0';

	if ((*ClientSocket).Write(sendbuf, type) == false)
	{
		free(sendbuf);
		return false;
	}
	free(sendbuf);

	if ((*ClientSocket).Read(MaxDLSize, type, NBytes) == false)
	{
		return false;
	}

	// early check the loading HTTP Header.
	// ensure that the received buffer won't crash the following functions
	/*
	* 1, see if it has HTTP/ header
	* 2, see if size of the data received greater than 12. ensure 3 digit status code exist 
	* 3, see if data reveived has string "\r\n\r\n". ensure HTTP header complete.
	*/
	if (strncmp("HTTP/", (*ClientSocket).buf, 5) || strlen((*ClientSocket).buf)<12 || strstr((*ClientSocket).buf,"\r\n\r\n") == NULL)
	{
		if (type != I_MT_FILE)
			printf("failed with non-HTTP header\n");
		return false;
	}

	t_finish = clock();

	if (type != I_MT_FILE)
		printf("done in %d ms with %d bytes\n", int(t_finish - t_start), (*ClientSocket).curPos);
	
	return true;
}

void PrintHttpHeader(Socket *ClientSocket)
{
	printf("\n----------------------------------------\n");
	char *EndOfHeader = strstr((*ClientSocket).buf,"\r\n\r\n");
	*EndOfHeader = '\0';
	printf((*ClientSocket).buf);
	printf("\n");
}

bool VerifyingHeader(Socket *ClientSocket, ValidStatus validcode, InputType type, int *HttpCode)
{
	if (type != I_MT_FILE)
		printf("\tVerifying header... ");
	char StatusCode[4] = { 0 };
	strncpy(StatusCode, ((*ClientSocket).buf + 9), 3); // 9 = strlen("HTTP/1.0") + 1
	if (type != I_MT_FILE)
		printf("status code %s\n", StatusCode);

	int status = atoi(StatusCode);
	*HttpCode = status;
	switch (validcode)
	{
	case TWO:
		if (status < 200 || status >= 300)
		{
			if (type == I_URL)
				PrintHttpHeader(ClientSocket);
			return false;
		}
		else
			return true;
		break;
	case FOUR:
		if (status < 400 || status >= 500)
		{
			return false;
		}
		else
			return true;
		break;
	default:
		printf("\tflase validcode argument");
		return false;
	}
	
}

bool SendHttpRequest(Socket *ClientSocket, RequestTarget target, ValidStatus validcode, char *StrHost, char *StrRequest,int MaxDLSize, InputType type, int *NBytes, int *HttpCode)
{
	bool ret;
	*HttpCode = 0; // invalid code
	*NBytes = 0;
	// refresh buffer in ClientSocket before every connection
	/*
	* 1, close the socket and open a new one
	* 2, check buffer size. realloc if needed 
	*/
	if ((*ClientSocket).Refresh() == false)
	{
		return false;
	}

	/* Connect on page */
	if ((ret = ConnectingOnPage(ClientSocket, target, type)) == false)
	{
		return false;
	}

	/* Loading the page */
	char *HTTPrequest = NULL;
	char *Request = NULL;
	switch (target)
	{
	case ROBOT:
		HTTPrequest = "HEAD %s HTTP/1.0\r\nUser-agent: celery_webcrawler/1.0\r\nHost: %s\r\n\r\n";
		Request = "/robots.txt";
		break;
	case PAGE:
		HTTPrequest = "GET %s HTTP/1.0\r\nUser-agent: celery_webcrawler/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n";
		Request = StrRequest;
		break;
	default:
		printf("\tflase target argument");
		return false;
	}
	if ((ret = Loading(ClientSocket, HTTPrequest, StrHost, Request, MaxDLSize, type, NBytes)) == false)
	{
		return false;
	}

	/* Verify received http header */
	if ((ret = VerifyingHeader(ClientSocket, validcode, type, HttpCode)) == false)
	{
		return false;
	}
	
	return true;
}

bool ParsingPage(HTMLParserBase *parser, Socket *ClientSocket, char *url, InputType type, int *NumLinks, bool *linkTAMU, bool *OutsideTAMU)
{
	clock_t t_start, t_finish;

	if (type != I_MT_FILE)
		printf("      + Parsing page... ");

	t_start = clock();

	char *baseUrl = NULL;
	if  ((baseUrl = (char *)malloc(strlen(url) + 1)) == NULL)
	{
		printf("failed with malloc error out of memory \n");
		return false;
	}
	memcpy(baseUrl, url, strlen(url));
	baseUrl[strlen(url)] = '\0';

	int nLinks = 0;
	char *body = strstr((*ClientSocket).buf, "\r\n\r\n") + 4;
	int HeaderSize = body - (*ClientSocket).buf;

	char *linkBuffer = parser->Parse(body, (*ClientSocket).curPos - HeaderSize, baseUrl, (int)strlen(baseUrl), &nLinks);

	t_finish = clock();

	/* for report */
	if (REPORT)
		FindTAMU(linkBuffer, nLinks, baseUrl, linkTAMU, OutsideTAMU);

	/* for report */

	*NumLinks = nLinks;
	if (type != I_MT_FILE)
		printf("done in %d ms with %d links\n", int(t_finish - t_start), nLinks);

	// free malloc space in heap
	free(baseUrl);

	if(type == I_URL)
		PrintHttpHeader(ClientSocket);
	return true;
}

void FindTAMU(char *linkBuffer, int nLinks, char *OriginalURL, bool *linkTAMU, bool *OutsideTAMU)
{
	char *currentLine = linkBuffer;
	char *nextLine = linkBuffer;
	char *slash = NULL;

	for (int i = 0; i < nLinks; i++) 
	{
		nextLine = strchr(currentLine, 0) + 1;
		if ((slash = strchr(currentLine + 7, '/')) != NULL) //remove the scheme and get the host.
			*slash = '\0';
		else
			slash = nextLine - 1;
		if (strcmp(slash - 8, "tamu.edu") == 0) // compare host end with tamu.edu (hard coded)
		{
			*linkTAMU = true;
			break;
		}
		currentLine = nextLine;
	}

	if ((slash = strchr(OriginalURL + 7, '/')) != NULL) //remove the scheme and get the host.
		*slash = '\0';
	else
		slash = strchr(OriginalURL, 0);
	if (strcmp(slash - 8, "tamu.edu") == 0) // compare host end with tamu.edu (hard coded)
	{
		*OutsideTAMU = false;
	}

}

char* FindEndOfLine(char *Str)
{
	char *pos = NULL;
	
	if ((pos = strstr(Str, "\r\n")) != NULL)
	{
		*pos = '\0';
		return pos + 2;
	}
	else if ((pos = strstr(Str, "\n")) != NULL)
	{
		*pos = '\0';
		return pos + 1;
	}
	else if (*Str == NULL)
		return NULL;
	else
	{
		return Str + strlen(Str);
	}
}

char* FindEndOfnLine(char *Str, int n)
{
	char *pos = Str;
	// check if the buffer is empty
	if (*Str == NULL) 
		return NULL;
	for (int i = 0; i < n; i++)
	{
		if (strstr(pos, "\n") == NULL) // same as strstr(pos, "\r\n") == NULL && strstr(pos, "\n") == NULL
			return Str + strlen(Str);
		else if ((pos = strstr(pos, "\r\n")) != NULL)
		{
			pos += 2;
		}
		else if ((pos = strstr(pos, "\n")) != NULL)
		{
			pos += 1;
		}
		else if (*pos == NULL)
		{
			break;
		}
	}
	if (*(pos - 2) == '\r')
	{
		*(pos - 2) = '\0';
	}
	else
	{
		*(pos - 1) = '\0';
	}

	return pos;
}