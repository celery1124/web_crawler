/* MySocket.cpp
* CSCE 613-600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

/***  Acknowledgment   ***/
/*
* Basic winsock usage referenced from sample code 463-sample
* select() usage referneced from MSDN
* http://msdn.microsoft.com/en-us/library/ms740141(VS.85).aspx 
*
*/

#include "MySocket.h"

Socket::Socket()
{
	// open a TCP socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// setup the port # and protocol type
	server.sin_family = AF_INET;

	//// allocate this buffer once, then possibly reuse for multiple connections in Part 3 
	buf = (char*)malloc(INITIAL_BUF_SIZE);    // start with 8 KB 
	allocatedSize = INITIAL_BUF_SIZE;
	curPos = 0;
}

Socket::~Socket()
{
	// free allocated memory space
	free(buf);    
	closesocket(sock);
}

void Socket::Initialize(IN_ADDR *IP_address, int Port)
{
	// setup the port # and IP
	server.sin_port = htons(Port);
	server.sin_addr = *IP_address;
}

bool Socket::SelfCheck()
{
	// check whether socket opened successfully
	if (sock == INVALID_SOCKET)
	{
		printf("failed with %d\n", WSAGetLastError());
		return false;
	}

	// check whether buf is allocated successfully
	if (buf == NULL)
	{
		printf("failed with malloc error out of memory \n");
		return false;
	}

	return true;
}

bool Socket::Connect(InputType type)
{
	// connect to the server 
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		if (type != I_MT_FILE)
			printf("failed with %d\n", WSAGetLastError());
		return false;
	}
	else
		return true;
}

bool Socket::Write(char *sendbuf, InputType type)
{
	int ret;
	ret = send(sock, sendbuf, (int)strlen(sendbuf), 0);
	if (ret == SOCKET_ERROR) {
		if (type != I_MT_FILE)
			printf("failed with: %d\n", WSAGetLastError());
		return false;
	}
	else
		return true;
}

bool Socket::Read(int MaxDLSize, InputType type, int *NBytes)
{ 
	int ret;
	// set timeout to 10 seconds
	clock_t time_elapse;
	const timeval timeout = { 10,0 };

	FD_SET readfds;

	time_elapse = clock();
	while (true)
	{
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		// wait to see if socket has any data (see MSDN) 
		if ((ret = select(0, &readfds, NULL, NULL, &timeout)) > 0)
		{
			// new data available; now read the next segment 
			int bytes = recv(sock, buf + curPos, allocatedSize - curPos, 0);

			if (bytes == SOCKET_ERROR)
			{
				// print WSAGetLastError() 
				if (type != I_MT_FILE)
					printf("failed with %d on recv \n", WSAGetLastError());
				break;
			}

			if (bytes == 0)
			{
				buf[curPos] = '\0';
				*NBytes = curPos;
				return true;    // normal completion 	
			}

			curPos += bytes;    // adjust where the next recv goes 

			if ((clock() - time_elapse) > MAX_LOAD_PAGE_TIME && type != I_URL)
			{
				if (type != I_MT_FILE)
					printf("failed with slow download\n");
				*NBytes = curPos;
				return false;
			}
			else if (curPos > MaxDLSize && type != I_URL)
			{
				if (type != I_MT_FILE)
					printf("failed with exceeding max\n");
				*NBytes = curPos;
				return false;
			}

			if ((allocatedSize - curPos) < THRESHOLD)
			{
				// resize buffer; besides STL, you can use  
				// realloc(), HeapReAlloc(), or memcpy the buffer  
				// into a bigger array 
				buf = (char*)realloc(buf, allocatedSize*2);
				allocatedSize *= 2;
			}
		}
		else if (ret == 0) {
			// report timeout 
			if (type != I_MT_FILE)
				printf("failed with receive timeout \n");
			break;
		}
		else {
			// print WSAGetLastError() 
			if (type != I_MT_FILE)
				printf("failed with %d on recv \n", WSAGetLastError());
			break;
		}
	}
	*NBytes = curPos;
	return false;
}


bool Socket::Refresh()
{
	closesocket(sock);
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// check whether socket opened successfully
	if (sock == INVALID_SOCKET)
	{
		printf("failed with %d\n", WSAGetLastError());
		return false;
	}

	if (allocatedSize > BUFFER_REFRESH_THRESHOLD)
	{
		free(buf);
		buf = (char*)malloc(INITIAL_BUF_SIZE);    // start with 8 KB 
		allocatedSize = INITIAL_BUF_SIZE;
	}
	curPos = 0;
	return true;
}