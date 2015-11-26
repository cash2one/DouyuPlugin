
// DouyuLocalDlg.h : 头文件
//

#pragma once
#include "DesStream.h"
#define CHADDRESS_LEN 20
#define USERNAMELEN 50
#define GIFTNAMELEN 20

#include <list>
using namespace std;

struct UserGiftRank
{
	int userid;
	wchar_t username[USERNAMELEN];
	int contribution;

	bool operator < (UserGiftRank &b) {	return contribution < b.contribution; }
};

struct UserGiftCount
{
	int userid;
	wchar_t username[USERNAMELEN];
	wchar_t giftname[GIFTNAMELEN];
	int giftcount;
};

struct UserGift
{
	time_t tm;
	int userid;
	wchar_t username[USERNAMELEN];
	wchar_t giftname[GIFTNAMELEN];
	int giftnum;
};

// CDouyuLocalDlg 对话框
class CDouyuLocalDlg : public CDHtmlDialog
{
public:
	CDesStream m_DesStream;
	BYTE m_svnf1, m_svnf2, m_svnf3, m_svnf4; /* data server ip */
	char m_chaddress[CHADDRESS_LEN];

	CString m_exeFolderPath;
	CString m_iniFilePath;
	void PrintText(LPCTSTR format, ...);

	void OnConnectionStateChanged();

	list<UserGift> m_listUserGift;
	list<UserGiftCount> m_listUserGiftCount;
	list<UserGiftRank> m_listUserGiftRank;

	void AddUserGift(int userid, CString struser, CString strgift, int giftnum);
	void AddUserChat(int userid, CString struser, CString strchat);
	int CalcContribution(CString strgift, int giftnum);
	CString GetGiftImage(CString strgift);

	bool m_bClosing;
	bool m_bAlwaysOnTop;

	virtual BOOL ContinueModal();

// 构造
public:
	CDouyuLocalDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DOUYULOCAL_DIALOG, IDH = IDR_HTML_DOUYULOCAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnHelpAbout();
	afx_msg void OnSetttingsServeraddress();
	afx_msg void OnFileConnecttoserver();

	afx_msg LRESULT OnDataStreamBreak(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataMsg(WPARAM wparam, LPARAM lparam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateFileConnecttoserver(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetttingsTopmostwindow(CCmdUI *pCmdUI);
	afx_msg void OnSetttingsTopmostwindow();
};
