
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "ChildView.h"
#define WCH_BUFFERSIZE 1024
#include <list>
using namespace std;

class CMainFrame : public CFrameWnd
{
public:
	SOCKET m_listensocket;
	HANDLE m_hlistenthread;
	u_short m_port;

	list<SOCKET> m_listStreamSocket;

	bool startlisten();
	void stoplisten();
	bool IsThreadSafeClosed();

	wchar_t m_recvData[WCH_BUFFERSIZE];

public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;
	CChildView    m_wndView;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);

	afx_msg LRESULT OnDataStreamConnected(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


