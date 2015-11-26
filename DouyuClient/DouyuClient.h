
// DouyuClient.h : DouyuClient 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define WM_DATASTREAMCONNECTED WM_USER + 299
#define WM_DATASTREAMBREAK WM_USER+300
#define WM_VERSIONERROR WM_USER+302
#define WM_DATAMSG    WM_USER + 310

void xASSERT(bool expression, int line, CString filepath);
bool safesend(SOCKET socket, char* pbuf, int len);
bool saferecv(SOCKET socket, char* pbuf, int len);

#ifdef UNICODE
#define _xASSERT(x) xASSERT(x,__LINE__,__FILEW__);
#else
#define _xASSERT(x) xASSERT(x,__LINE__,__FILE__);
#endif // UNICODE

// CDouyuClientApp:
// 有关此类的实现，请参阅 DouyuClient.cpp
//

class CDouyuClientApp : public CWinApp
{
public:
	CDouyuClientApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDouyuClientApp theApp;
