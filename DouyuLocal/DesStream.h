#pragma once

class CDouyuLocalDlg;

class CDesStream
{
public:
	CDouyuLocalDlg *m_pDlg;

	bool m_bconnected;
	SOCKET m_dessocket;
	HANDLE m_hdesthread;
	u_short m_port;

	bool ConnectToServer(const char* chaddress);
	bool IsThreadSafeClosed();

public:
	CDesStream();
	~CDesStream();
};

