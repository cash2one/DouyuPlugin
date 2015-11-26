#pragma once


// CMsgListCtrl

class CMsgListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMsgListCtrl)
public:
	void AddMessage(CString strmsg);

public:
	CMsgListCtrl();
	virtual ~CMsgListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};


