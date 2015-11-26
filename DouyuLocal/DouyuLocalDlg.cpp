
// DouyuLocalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DouyuLocal.h"
#include "DouyuLocalDlg.h"
#include "afxdialogex.h"

#include "ServerAddressDlg.h"
#define TIMER_QUITMSG 201

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDouyuLocalDlg �Ի���

BEGIN_DHTML_EVENT_MAP(CDouyuLocalDlg)


END_DHTML_EVENT_MAP()


CDouyuLocalDlg::CDouyuLocalDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDouyuLocalDlg::IDD, CDouyuLocalDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/* ��ȡ����·��*/
	wchar_t exePath[MAX_PATH];
	GetModuleFileName(NULL, exePath, MAX_PATH);
	CString exefolderpath = exePath;
	int pos = exefolderpath.ReverseFind(L'\\');
	exefolderpath = exefolderpath.Left(pos + 1); /*����б��*/
	m_exeFolderPath = exefolderpath;

	m_iniFilePath = exefolderpath + L"settings.ini";

	m_svnf1 = GetPrivateProfileInt(L"DataServer", L"NF1", 192, m_iniFilePath);
	m_svnf2 = GetPrivateProfileInt(L"DataServer", L"NF2", 168, m_iniFilePath);
	m_svnf3 = GetPrivateProfileInt(L"DataServer", L"NF3", 0, m_iniFilePath);
	m_svnf4 = GetPrivateProfileInt(L"DataServer", L"NF4", 100, m_iniFilePath);
	sprintf(m_chaddress, "%d.%d.%d.%d", m_svnf1, m_svnf2, m_svnf3, m_svnf4);

	m_DesStream.m_pDlg = this;
	m_bClosing = false;

}

void CDouyuLocalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDouyuLocalDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_HELP_ABOUT, &CDouyuLocalDlg::OnHelpAbout)
	ON_COMMAND(ID_SETTTINGS_SERVERADDRESS, &CDouyuLocalDlg::OnSetttingsServeraddress)
	ON_COMMAND(ID_FILE_CONNECTTOSERVER, &CDouyuLocalDlg::OnFileConnecttoserver)
	ON_MESSAGE(WM_DATASTREAMBREAK, &CDouyuLocalDlg::OnDataStreamBreak)
	ON_MESSAGE(WM_DATAMSG, &CDouyuLocalDlg::OnDataMsg)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FILE_CONNECTTOSERVER, &CDouyuLocalDlg::OnUpdateFileConnecttoserver)
END_MESSAGE_MAP()


// CDouyuLocalDlg ��Ϣ�������

BOOL CDouyuLocalDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return FALSE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDouyuLocalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDouyuLocalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDouyuLocalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDouyuLocalDlg::OnHelpAbout()
{
	CComQIPtr<IHTMLDocument2> phtmlDoc;
	GetDHtmlDocument(&phtmlDoc);

	CComDispatchDriver spScript;
	phtmlDoc->get_Script(&spScript);

	CComVariant varStr = _T("<span style=\"font-family: Georgia, serif; font-size:12px;\">лл���͵�100����</span>");
	CComVariant varRet;
	spScript.Invoke2(_T("addInnerHTML"), &varStr, &varRet);

}


void CDouyuLocalDlg::OnSetttingsServeraddress()
{
	CServerAddressDlg dlg;
	dlg.m_svnf1 = m_svnf1;
	dlg.m_svnf2 = m_svnf2;
	dlg.m_svnf3 = m_svnf3;
	dlg.m_svnf4 = m_svnf4;

	if (dlg.DoModal() == IDOK)
	{
		m_svnf1 = dlg.m_svnf1;
		m_svnf2 = dlg.m_svnf2;
		m_svnf3 = dlg.m_svnf3;
		m_svnf4 = dlg.m_svnf4;
		sprintf(m_chaddress, "%d.%d.%d.%d", m_svnf1, m_svnf2, m_svnf3, m_svnf4);

		CString str;
		str.Format(L"%d", m_svnf1);
		WritePrivateProfileStringW(L"DataServer", L"NF1", str, m_iniFilePath);
		str.Format(L"%d", m_svnf2);
		WritePrivateProfileStringW(L"DataServer", L"NF2", str, m_iniFilePath);
		str.Format(L"%d", m_svnf3);
		WritePrivateProfileStringW(L"DataServer", L"NF3", str, m_iniFilePath);
		str.Format(L"%d", m_svnf4);
		WritePrivateProfileStringW(L"DataServer", L"NF4", str, m_iniFilePath);
	}

}


void CDouyuLocalDlg::OnFileConnecttoserver()
{
	if (m_DesStream.m_bconnected)
	{
		MessageBeep(1);
		return;
	}

	if (m_DesStream.ConnectToServer(m_chaddress))
	{
		PrintText(L"�Ѿ����ӵ�Ļ��������");
		OnConnectionStateChanged();
	}
	else
	{
		PrintText(L"���ӵ�Ļ������ʧ�ܡ�");
		OnConnectionStateChanged();
	}

}

void CDouyuLocalDlg::PrintText(LPCTSTR format, ...)
{
	CString str, strtm;
	CTime tm = CTime::GetCurrentTime();
	strtm = tm.Format(L"%Y.%m.%d %H:%M:%S");

	va_list args;
	va_start(args, format);
	str.FormatV(format, args);
	va_end(args);

	CComQIPtr<IHTMLDocument2> phtmlDoc;
	GetDHtmlDocument(&phtmlDoc);

	CComDispatchDriver spScript;
	phtmlDoc->get_Script(&spScript);

	CComVariant varStr = str;
	CComVariant varRet;
	spScript.Invoke2(_T("addInnerHTML"), &varStr, &varRet);
}

void CDouyuLocalDlg::OnConnectionStateChanged()
{
	CString strimg;
	if (m_DesStream.m_bconnected)
	{
		strimg.Format(L"��Ļ������״̬��<img src=\"%s\" />", m_exeFolderPath + L"images\\green_light.png");
	}
	else
	{
		strimg.Format(L"��Ļ������״̬��<img src=\"%s\" />", m_exeFolderPath + L"images\\red_light.png");
	}

	CComQIPtr<IHTMLDocument2> phtmlDoc;
	GetDHtmlDocument(&phtmlDoc);

	CComDispatchDriver spScript;
	phtmlDoc->get_Script(&spScript);

	CComVariant varStr = strimg;
	CComVariant varRet;
	spScript.Invoke2(_T("changeStatImg"), &varStr, &varRet);

}

void CDouyuLocalDlg::AddUserGift(int userid, CString struser, CString strgift, int giftnum)
{
	CTime curCtm = CTime::GetCurrentTime();

	UserGift onegift;
	onegift.tm = curCtm.GetTime();
	onegift.userid = userid;
	wcscpy(onegift.username, struser);
	wcscpy(onegift.giftname, strgift);
	onegift.giftnum = giftnum;
	m_listUserGift.push_back(onegift);

	list<UserGiftCount>::iterator itugc;
	bool bFound = false;
	int giftcount = 0;
	for (itugc = m_listUserGiftCount.begin(); itugc != m_listUserGiftCount.end(); itugc++)
	{
		//if (itugc->userid == userid && StrCmp(itugc->giftname, strgift) == 0)
		if (StrCmp(itugc->giftname, strgift) == 0 && StrCmp(itugc->username, struser) == 0)
		{
			bFound = true;
			itugc->giftcount += giftnum;
			//wcscpy(itugc->username, struser);
			giftcount = itugc->giftcount;
			break;
		}
	}

	if (!bFound)
	{
		UserGiftCount ugc;
		ugc.userid = userid;
		wcscpy(ugc.giftname, strgift);
		wcscpy(ugc.username, struser);
		ugc.giftcount = giftnum;
		giftcount = ugc.giftcount;
		m_listUserGiftCount.push_back(ugc);
	}

	int contribution = CalcContribution(strgift, giftnum);
	list<UserGiftRank>::iterator itugr;
	bFound = false;
	for (itugr = m_listUserGiftRank.begin(); itugr != m_listUserGiftRank.end(); itugr++)
	{
		//if (itugr->userid == userid)
		if (StrCmp(itugr->username, struser) == 0)
		{
			itugr->contribution += contribution;
			wcscpy(itugr->username, struser);
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		UserGiftRank ugr;
		ugr.userid = userid;
		wcscpy(ugr.username, struser);
		ugr.contribution = contribution;
		m_listUserGiftRank.push_back(ugr);
	}

	m_listUserGiftRank.sort();

	/* add msg */
	if (StrCmp(strgift, L"100����") == 0)
	{
		CString strmsg;
		strmsg.Format(L"<span style=\"color:blue\">%s</span>���͸�����%d00������<img src=\"%s\" />%d����", 
			struser, giftnum, m_exeFolderPath + L"images\\yuwan.jpg", giftcount);

		PrintText(strmsg);
	}
	else
	{
		CString strmsg;
		strmsg.Format(L"<span style=\"color:blue\">%s</span>���͸�����%d��%s<img src=\"%s\" />%d����",
			struser, giftnum, m_exeFolderPath + L"images\\" + GetGiftImage(strgift), giftcount);

		PrintText(strmsg);
	}


}

int CDouyuLocalDlg::CalcContribution(CString strgift, int giftnum)
{
	int contribution = 100;

	if (StrCmp(strgift, L"100����") == 0)
	{
		contribution = 100;
	}
	else if (StrCmp(strgift, L"520") == 0)
	{
		contribution = 520;
	}
	else if (StrCmp(strgift, L"��") == 0)
	{
		contribution = 100;
	}
	else if (StrCmp(strgift, L"666") == 0)
	{
		contribution = 6000;
	}
	else if (StrCmp(strgift, L"�ɻ�") == 0)
	{
		contribution = 100000;
	}
	else if (StrCmp(strgift, L"���") == 0)
	{
		contribution = 500000;
	}
	else if (StrCmp(strgift, L"��������") == 0)
	{
		contribution = 15000;
	}
	else if (StrCmp(strgift, L"�м�����") == 0)
	{
		contribution = 30000;
	}
	else if (StrCmp(strgift, L"�߼�����") == 0)
	{
		contribution = 50000;
	}

	return contribution;
}

CString CDouyuLocalDlg::GetGiftImage(CString strgift)
{
	CString strImageFileName = L"yuwan.jpg";
	if (StrCmp(strgift, L"100����") == 0)
	{
		strImageFileName = L"yuwan.jpg";
	}
	else if (StrCmp(strgift, L"520") == 0)
	{
		strImageFileName = L"520.gif";
	}
	else if (StrCmp(strgift, L"��") == 0)
	{
		strImageFileName = L"zan.png";
	}
	else if (StrCmp(strgift, L"666") == 0)
	{
		strImageFileName = L"666.png";
	}
	else if (StrCmp(strgift, L"�ɻ�") == 0)
	{
		strImageFileName = L"feiji.png";
	}
	else if (StrCmp(strgift, L"���") == 0)
	{
		strImageFileName = L"huojian.png";
	}
	else if (StrCmp(strgift, L"��������") == 0)
	{
		strImageFileName = L"cq_lv01.png";
	}
	else if (StrCmp(strgift, L"�м�����") == 0)
	{
		strImageFileName = L"cq_lv02.png";
	}
	else if (StrCmp(strgift, L"�߼�����") == 0)
	{
		strImageFileName = L"cq_lv03.png";
	}

	return strImageFileName;
}

void CDouyuLocalDlg::AddUserChat(int userid, CString struser, CString strchat)
{
	CString strmsg;
	strmsg.Format(L"<span style=\"color:blue\">%s: </span>%s", struser, strchat);

	PrintText(strmsg);
}

LRESULT CDouyuLocalDlg::OnDataStreamBreak(WPARAM wparam, LPARAM lparam)
{
	if (m_bClosing) return 0;

	_xASSERT(m_DesStream.m_bconnected);
	m_DesStream.m_bconnected = false;

	PrintText(L"�뵯Ļ�������������ѶϿ���");
	OnConnectionStateChanged();

	return 1;
}

LRESULT CDouyuLocalDlg::OnDataMsg(WPARAM wparam, LPARAM lparam)
{
	wchar_t* pwch = (wchar_t*)wparam;
	CString strmsg = pwch;

	CString strsplit = L"(,)";
	CString strid, strname, strtype, strchat, strgiftname, strgiftnum;

	int pos = strmsg.Find(strsplit);
	if (pos < 0) return -1;

	strid = strmsg.Left(pos);
	strmsg = strmsg.Mid(pos + 3);
	int userid = _wtoi(strid);

	pos = strmsg.Find(strsplit);
	strname = strmsg.Left(pos);
	strmsg = strmsg.Mid(pos + 3);

	pos = strmsg.Find(strsplit);
	if (pos < 0) return -1;
	strtype = strmsg.Left(pos);
	strmsg = strmsg.Mid(pos + 3);

	if (StrCmp(strtype, L"chat")==0)
	{
		strchat = strmsg;
		AddUserChat(userid, strname, strchat);
	}
	else if (StrCmp(strtype, L"gift") == 0)
	{
		pos = strmsg.Find(strsplit);
		if (pos < 0) return -1;

		strgiftname = strmsg.Left(pos);
		strgiftnum = strmsg.Mid(pos + 3);

		AddUserGift(userid, strname, strgiftname, _wtoi(strgiftnum));
	}
	else
	{
		return -1;
	}

}



void CDouyuLocalDlg::OnClose()
{
	closesocket(m_DesStream.m_dessocket);
	SetTimer(TIMER_QUITMSG, 200, NULL);
	
}


void CDouyuLocalDlg::OnTimer(UINT_PTR nIDEvent)
{
	while (nIDEvent == TIMER_QUITMSG) /* not true loop */
	{
		if (!m_DesStream.IsThreadSafeClosed())
			break;

		KillTimer(nIDEvent);
		WSACleanup();
		CDHtmlDialog::OnCancel();
		return;

	}

	CDHtmlDialog::OnTimer(nIDEvent);
}


void CDouyuLocalDlg::OnUpdateFileConnecttoserver(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_DesStream.m_bconnected ? FALSE : TRUE);
}

BOOL CDouyuLocalDlg::ContinueModal()
{
	CMenu* pMainMenu = GetMenu();
	CCmdUI cmdUI;
	for (UINT n = 0; n < pMainMenu->GetMenuItemCount(); ++n)
	{
		CMenu* pSubMenu = pMainMenu->GetSubMenu(n);
		cmdUI.m_nIndexMax = pSubMenu->GetMenuItemCount();
		for (UINT i = 0; i < cmdUI.m_nIndexMax; ++i)
		{
			cmdUI.m_nIndex = i;
			cmdUI.m_nID = pSubMenu->GetMenuItemID(i);
			cmdUI.m_pMenu = pSubMenu;
			cmdUI.DoUpdate(this, FALSE);
		}
	}

	return CDHtmlDialog::ContinueModal();
}
