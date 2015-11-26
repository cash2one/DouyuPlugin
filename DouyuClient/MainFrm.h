
// MainFrm.h : CMainFrame 类的接口
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

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;
	CChildView    m_wndView;

// 生成的消息映射函数
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


