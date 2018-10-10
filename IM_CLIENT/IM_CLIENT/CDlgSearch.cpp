// CDlgSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgSearch.h"
#include "CDlgUserInfo.h"
#include "afxdialogex.h"


// CDlgSearch 对话框

IMPLEMENT_DYNAMIC(CDlgSearch, CDialogEx)


CDlgSearch::CDlgSearch(CClientSocket * client, bool isuid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEARCH, pParent)
	, m_strid(_T(""))
{
	m_client = client;
	m_bIsUid = isuid;
}



CDlgSearch::~CDlgSearch()
{
}

void CDlgSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strid);
	DDX_Control(pDX, IDC_SEARCHLIST, m_ListSearch);
}


void CDlgSearch::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_bIsUid)
	{
		CString su = "SearchUser";
		m_client->m_map.erase(su);
	}
	else
	{
		CString sg = "SearchGroup";
		m_client->m_map.erase(sg);
	}
	CDialogEx::OnCancel();
}

void CDlgSearch::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//搜索用户
	if (m_bIsUid)
	{
		if (m_strid == m_client->m_szUID)
		{
			MessageBox("不能搜索自己");
			return;
		}

		//CStringA str = CW2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		CStringA str = CT2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		m_client->Send(SEARCHUSER, str.GetBuffer(), 1);
	}
	else
	{
		//CStringA str = CW2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		CStringA str = CT2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		m_client->Send(SEARCHGROUP, str.GetBuffer(), 1);
	}

	//CDialogEx::OnOK();
}

void CDlgSearch::OnNMRClickSearchlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	//pMenu->LoadMenuW(IDR_MENU1);
	pMenu->LoadMenu(IDR_MENU1);

	CMenu* pSub = NULL;

	//判断搜索的是用户还是群
	if (m_bIsUid)
	{
		pSub = pMenu->GetSubMenu(0);
	}
	else
	{
		pSub = pMenu->GetSubMenu(1);
	}
	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}

void CDlgSearch::On32771_AddFriend()
{
	// TODO: 在此添加命令处理程序代码
	int nSel = m_ListSearch.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("请选中数据");
		return;
	}
	CString uid = m_ListSearch.GetItemText(nSel, 0);
	if (uid == m_client->m_szUID)
	{
		MessageBox("不能添加自己");
		return;
	}

	//CStringA str = CW2A(uid.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(uid.GetBuffer(), CP_THREAD_ACP);
	m_client->Send(ADDFRIEND, str.GetBuffer(), 1);
}

void CDlgSearch::On32772_ShowUserInfo()
{
	// TODO: 在此添加命令处理程序代码
	//获取信息
	int nSel = m_ListSearch.GetSelectionMark();
	CString uid = m_ListSearch.GetItemText(nSel, 0);
	CString uname = m_ListSearch.GetItemText(nSel, 1);

	//显示信息
	CDlgUserInfo *UserInfo = new CDlgUserInfo(m_client, uid, uname);
	UserInfo->Create(IDD_USERINFO);
	UserInfo->ShowWindow(SW_NORMAL);
}

BEGIN_MESSAGE_MAP(CDlgSearch, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSearch::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_SEARCHLIST, &CDlgSearch::OnNMRClickSearchlist)
	ON_COMMAND(ID_32771, &CDlgSearch::On32771_AddFriend)
	ON_COMMAND(ID_32772, &CDlgSearch::On32772_ShowUserInfo)
END_MESSAGE_MAP()


// CDlgSearch 消息处理程序
