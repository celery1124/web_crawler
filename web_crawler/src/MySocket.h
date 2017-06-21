/* MySocket.h
* CSCE 613 - 600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

#ifndef   _my_socket_h_   
#define   _my_socket_h_   

#include "common.h"
#include <stdio.h>
#include <winsock2.h>

#define INITIAL_BUF_SIZE 8196
#define THRESHOLD 1024
#define BUFFER_REFRESH_THRESHOLD 32768
#define MAX_LOAD_PAGE_TIME 10000 // unit: ms

typedef enum { ROBOT, PAGE } RequestTarget;
typedef enum { TWO, FOUR } ValidStatus;
typedef enum { I_URL, I_FILE, I_MT_FILE } InputType;

class Socket
{
	public:
		SOCKET  sock;      // socket handle 
		sockaddr_in server; // server 
		char   *buf;      // current buffer 
		int  allocatedSize;   // bytes allocated for buf 
		int  curPos;     // current position in buffer 

		Socket();
		~Socket();

		void Initialize(IN_ADDR *IP_address, int Port);
		bool SelfCheck();
		bool Connect(InputType type);
		bool Write(char *sendbuf, InputType type);
		bool Read(int MaxDLSize, InputType type, int *NBytes);
		bool Refresh();
};

#endif 