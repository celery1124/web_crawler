/* StepFunction.h
* CSCE 613 - 600 Spring 2017
* HW1 web_crawler
* by Mian Qin
*/

#ifndef   _step_function_h_   
#define   _step_function_h_

#include "common.h"
#include "MySocket.h"
#include "MySet.h"
#include "stdafx.h"

/* ParsingURL
* filename [in]
* fileBuf [out]
*/
bool OpenFile(char *filename, char **fileBuf);

/* ParsingURL
* url [in]
* StrHost [out] 
* Port [out]
* StrRequest [out]
*/
bool ParsingURL(char *Inputurl, char *StrHost, int *Port, char *StrRequest, InputType type);

/* CheckHostUnique
* seenHosts [in]
* test [in]
*/
bool CheckHostUnique(HostSet *seenHosts, string test, InputType type);

/* CheckIPUnique
* seenIPs [in]
* test [in]
*/
bool CheckIPUnique(IPSet *seenIPs, DWORD test, InputType type);

/* DoingDNS
* StrHost [in]
* IP_address [out]
*/
bool DoingDNS(char *StrHost, IN_ADDR *IP_address, InputType type);

/* ConnectingOnPage
* ClientSocket [in]
* target [in]
*/
bool ConnectingOnPage(Socket *ClientSocket, RequestTarget target, InputType type);

/* Loading
* ClientSocket [in]
* HTTPrequest [in]
* StrHost [in]
* StrRequest [in]
* MaxDLSize[in]
* type [in]
*/
bool Loading(Socket *ClientSocket, const char* HTTPrequest, char *StrHost, char *StrRequest, int MaxDLSize, InputType type, int *NBytes);

/* VerifyingHeader
* ClientSocket [in]
* validcode [in]
* type [in]
*/
bool VerifyingHeader(Socket *ClientSocket, ValidStatus validcode, InputType type, int *HttpCode);

/* PrintHttpHeader
* ClientSocket [in]
*/
void PrintHttpHeader(Socket *ClientSocket);

/* SendHttpRequest
* ClientSocket [in]
* target [in]
* validcode [in]
* StrHost [in]
* StrRequest [in]
* MaxDLSize[in]
* type [in]
*/
bool SendHttpRequest(Socket *ClientSocket, RequestTarget target, ValidStatus validcode, char *StrHost, char *StrRequest, int MaxDLSize, InputType type, int *NBytes, int *HttpCode);

/* ParsingPage
* ClientSocket [in]
* url [in]
* type [in]
*/
bool ParsingPage(HTMLParserBase *parser, Socket *ClientSocket, char *url, InputType type, int *NumLinks, bool *linkTAMU, bool *OutsideTAMU);

void FindTAMU(char *linkBuffer, int nLinks, char *OriginalURL, bool *linkTAMU, bool *OutsideTAMU);

/* FindEndOfLine
* Str [in]
* return start postion of nextline
*/
char* FindEndOfLine(char *Str);
char* FindEndOfnLine(char *Str, int n);
#endif