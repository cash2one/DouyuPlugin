
// DouyuLocal.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

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

// CDouyuLocalApp: 
// �йش����ʵ�֣������ DouyuLocal.cpp
//

class CDouyuLocalApp : public CWinApp
{
public:
	CDouyuLocalApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDouyuLocalApp theApp;