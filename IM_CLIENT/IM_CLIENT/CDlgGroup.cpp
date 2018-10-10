// CDlgGroup.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgGroup.h"
#include"CDlgGroupChat.h"
#include "afxdialogex.h"


// CDlgGroup 对话框

IMPLEMENT_DYNAMIC(CDlgGroup, CDialogEx)


CDlgGroup::CDlgGroup(CClientSocket * client, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_GROUP, pParent)
{
	m_client = client;
}



CDlgGroup::~CDlgGroup()
{
}

void CDlgGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GROUP, m_GList);
}


BOOL CDlgGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//MoveWindow(0, 0, 365, 440, 1);

	// TODO:  在此添加额外的初始化
	m_GList.AddColumn(1, L"群号", 200);

	//更新群列表
	UpdataGroup();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgGroup::UpdataGroup()
{
	//请求群列表;	

	m_client->Send(UPDATEGROUP, m_client->m_szUID, strlen(m_client->m_szUID));

	char *ret = m_client->Recv();

	//循环接收
	while (strcmp(ret, "~~~end~~~"))
	{
		//CString str = CA2W(ret, CP_THREAD_ACP);
		CString str = CA2T(ret, CP_THREAD_ACP);
		m_GList.AddItem(1, str);
		ret = m_client->Recv();
	}
}


//双击打开群聊
void CDlgGroup::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取选中项;
	int nSel = m_GList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("请选中数据");
		return;
	}
	CString gid = m_GList.GetItemText(nSel, 0);

	if (m_client->m_map.find(gid) == m_client->m_map.end())
	{
		//找不到对应窗口就创建
		CDlgGroupChat *GroupChat = new CDlgGroupChat(m_client, gid, this);

		GroupChat->Create(IDD_GROUPCHAT);
		GroupChat->ShowWindow(SW_NORMAL);
		//GroupChat->SetWindowTextW(gid);
		GroupChat->SetWindowText(gid);
		m_client->m_map[gid] = GroupChat;
		GroupChat->UpdataGroupMember();

	}
	else
	{
		//显示聊天窗口
		CDlgGroupChat *GroupChat = (CDlgGroupChat *)m_client->m_map[gid];
		GroupChat->ShowWindow(SW_NORMAL);
		GroupChat->UpdataGroupMember();
	}

	*pResult = 0;
}

BEGIN_MESSAGE_MAP(CDlgGroup, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUP, &CDlgGroup::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgGroup 消息处理程序
