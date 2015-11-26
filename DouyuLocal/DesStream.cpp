#include "stdafx.h"
#include "DesStream.h"
#include "DouyuLocal.h"
#include "DouyuLocalDlg.h"

#include <tchar.h>
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define BUFFERLEN 1024
#define VERSIONNUMBER 20151109
#define DISCONNECTDATASTREAM closesocket(socket);socket = NULL;SendMessage(hmainwnd, WM_DATASTREAMBREAK, 0,0);

DWORD WINAPI DesStreamThread(LPVOID lpParameter)
{
	CDesStream* pds = (CDesStream*)lpParameter;
	HWND hmainwnd = pds->m_pDlg->GetSafeHwnd();
	SOCKET &socket = pds->m_dessocket;
	unsigned char uc = 0;

	char ch[20] = "douyudata";
	if (!safesend(socket, ch, 20)) { DISCONNECTDATASTREAM; return -1; }

	int version;
	if (!saferecv(socket, (char*)&version, sizeof(int))) { DISCONNECTDATASTREAM; return -1; }
	if (version != VERSIONNUMBER)
	{
		DISCONNECTDATASTREAM; return -1;
	}

	int datalen = 0;
	char buffer[BUFFERLEN];

	while (true)
	{
		if (!saferecv(socket, (char*)&datalen, sizeof(int))) { DISCONNECTDATASTREAM; return -1; }
		if (datalen <= 0 || datalen > BUFFERLEN)
		{
			DISCONNECTDATASTREAM; break;
		}
		
		if (!saferecv(socket, buffer, datalen)) { DISCONNECTDATASTREAM; return -1; }
		SendMessage(hmainwnd, WM_DATAMSG, (WPARAM)buffer, (LPARAM) 0);

	}

	return 1;
}

CDesStream::CDesStream()
{
	m_pDlg = NULL;
	m_bconnected = false;
	m_dessocket = NULL;
	m_hdesthread = NULL;
	m_port = 19237;
}


CDesStream::~CDesStream()
{
}

bool CDesStream::ConnectToServer(const char* chaddress)
{
	_xASSERT(m_pDlg);
	if (m_bconnected) return false;

	unsigned long ip;
	if ((ip = inet_addr(chaddress)) == INADDR_NONE) { return false; }

	if ((m_dessocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) { return false; }
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = ip;
	serverAddress.sin_port = htons(m_port);

	timeval tm;
	fd_set set;
	int error = -1;
	int len = sizeof(int);
	unsigned long ul = 1;
	ioctlsocket(m_dessocket, FIONBIO, &ul); /* 设置为非阻塞模式 */
	bool ret = false;
	if (connect(m_dessocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		tm.tv_sec = 1;
		tm.tv_usec = 200000;
		FD_ZERO(&set);
		FD_SET(m_dessocket, &set);
		if (select(m_dessocket + 1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(m_dessocket, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
			if (error == 0)
				ret = true;
			else
				ret = false;
		}
		else
			ret = false;

	}
	else
		ret = true;

	ul = 0;
	ioctlsocket(m_dessocket, FIONBIO, &ul); /* 设置为阻塞模式 */

	if (!ret)
	{
		closesocket(m_dessocket);
		m_dessocket = NULL;
		return false;
	}

	m_bconnected = true;
	m_hdesthread = CreateThread(NULL, 0, DesStreamThread, (LPVOID)this, 0, NULL);
	return true;

}

bool CDesStream::IsThreadSafeClosed()
{
	if (m_hdesthread == NULL)
		return true;

	if (WaitForSingleObject(m_hdesthread, 0) == WAIT_OBJECT_0)
	{
		CloseHandle(m_hdesthread);
		m_hdesthread = NULL;
		return true;
	}

	return false;
}


