
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "DouyuClient.h"

#include "MainFrm.h"
#define TIMER_QUITMSG 201


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_COPYDATA()
	ON_MESSAGE(WM_DATASTREAMCONNECTED, &CMainFrame::OnDataStreamConnected)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	m_listensocket = NULL;
	m_hlistenthread = NULL;
	m_port = 19237;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(L"DouyuPluginClient");

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	startlisten();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	cs.cx = 1080;
	cs.cy = 600;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if (pCopyDataStruct->cbData > 0)
	{
		memcpy((void*)m_recvData, (void*)pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		
		CString strmsg = m_recvData;
		m_wndView.m_listctrl.AddMessage(strmsg);

		int datalen = (strmsg.GetLength() + 1) * sizeof(wchar_t);

		list<SOCKET>::iterator itsocket;
		for (itsocket = m_listStreamSocket.begin(); itsocket != m_listStreamSocket.end();)
		{
			SOCKET socket = *itsocket;
			if (!safesend(socket, (char*)&datalen, sizeof(int)))
			{
				closesocket(socket);
				itsocket = m_listStreamSocket.erase(itsocket);
				continue;
			}

			if (!safesend(socket, (char*)m_recvData, datalen))
			{
				closesocket(socket);
				itsocket = m_listStreamSocket.erase(itsocket);
				continue;
			}

			itsocket++;
			
		}
	}

	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

#define VERSIONNUMBER 20151109

DWORD WINAPI clientlistenthread(LPVOID lpParameter)
{
	CMainFrame *pfrm = (CMainFrame*)lpParameter;
	int AddrLen = 0;
	sockaddr_in ClientAddr;

	while (true)
	{
		AddrLen = sizeof(ClientAddr);
		SOCKET clientsocket = accept(pfrm->m_listensocket, (struct sockaddr*)&ClientAddr, &AddrLen);
		if (clientsocket == INVALID_SOCKET)
			break;

		char ch[20];
		if (!saferecv(clientsocket, ch, 10)) { closesocket(clientsocket); continue; }
		if (strcmp(ch, "douyudata") != 0) { closesocket(clientsocket); continue; }
		int version = VERSIONNUMBER;
		if (!safesend(clientsocket, (char*)&version, sizeof(version))) { closesocket(clientsocket); continue; }

		pfrm->SendMessage(WM_DATASTREAMCONNECTED, (WPARAM)clientsocket, 0);

	}

	return 1;
}

bool CMainFrame::startlisten()
{
	m_listensocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listensocket == INVALID_SOCKET)
		return false;

	sockaddr_in LocalAddr;
	LocalAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(m_port);

	if (bind(m_listensocket, (sockaddr *)&LocalAddr, sizeof(LocalAddr)) != 0)
		return false;

	if (listen(m_listensocket, 20) != 0)
		return false;

	m_hlistenthread = CreateThread(NULL, 0, clientlistenthread, (LPVOID)this, 0, NULL);
	_xASSERT(m_hlistenthread != NULL);
	return true;
}

void CMainFrame::stoplisten()
{
	closesocket(m_listensocket);
}

bool CMainFrame::IsThreadSafeClosed()
{
	if (m_hlistenthread == NULL)
		return true;

	if (WaitForSingleObject(m_hlistenthread, 0) == WAIT_OBJECT_0)
	{
		CloseHandle(m_hlistenthread);
		m_hlistenthread = NULL;
		return true;
	}

	return false;

}


LRESULT CMainFrame::OnDataStreamConnected(WPARAM wparam, LPARAM lparam)
{
	m_listStreamSocket.push_back((SOCKET)wparam);
	return 1;
}


void CMainFrame::OnClose()
{
	if (MessageBox(L"Are you sure to quit?", L"Confirm", MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	list<SOCKET>::iterator itsocket;
	for (itsocket = m_listStreamSocket.begin(); itsocket != m_listStreamSocket.end();)
	{
		SOCKET socket = *itsocket;
		closesocket(socket);
		itsocket = m_listStreamSocket.erase(itsocket);
	}

	stoplisten();
	SetTimer(TIMER_QUITMSG, 100, NULL);
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	while (nIDEvent == TIMER_QUITMSG) /* not true loop */
	{
		if (!IsThreadSafeClosed())
			break;

		KillTimer(nIDEvent);
		WSACleanup();
		CFrameWnd::OnClose();
		return;
	}

	CFrameWnd::OnTimer(nIDEvent);
}
