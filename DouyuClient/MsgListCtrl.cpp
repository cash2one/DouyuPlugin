// MsgListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DouyuClient.h"
#include "MsgListCtrl.h"


// CMsgListCtrl

IMPLEMENT_DYNAMIC(CMsgListCtrl, CListCtrl)

CMsgListCtrl::CMsgListCtrl()
{

}

CMsgListCtrl::~CMsgListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMsgListCtrl, CListCtrl)
END_MESSAGE_MAP()

void CMsgListCtrl::AddMessage(CString strmsg)
{
	CString strtm;
	CTime tm = CTime::GetCurrentTime();
	strtm = tm.Format("%Y.%m.%d %H:%M:%S");

	int nItem = InsertItem(GetItemCount(), strtm);
	SetItemText(nItem, 1, strmsg);

	EnsureVisible(0, FALSE);
}



// CMsgListCtrl message handlers


