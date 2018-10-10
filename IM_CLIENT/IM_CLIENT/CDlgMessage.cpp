// CDlgMessage.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgMessage.h"
#include "CDlgOne2One.h"
#include "afxdialogex.h"


// CDlgMessage 对话框

IMPLEMENT_DYNAMIC(CDlgMessage, CDialogEx)

CDlgMessage::CDlgMessage(CClientSocket * client, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MSG, pParent)
{
	m_client = client;
}

CDlgMessage::~CDlgMessage()
{
}

void CDlgMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LASTCHAT, m_LastChatList);
}


BOOL CDlgMessage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//MoveWindow(0, 0, 365, 440, 1);

	m_LastChatList.AddColumn(1, "好友", 200);
	CString str = "LastChat";
	m_client->m_map[str] = this;

	//从文件读取列表
	ReadChatList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CDlgMessage, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LASTCHAT, &CDlgMessage::OnNMDblclkLastchat)
END_MESSAGE_MAP()


// CDlgMessage 消息处理程序


void CDlgMessage::UpDataChatList(CString fid)
{
	//删除列表内已存在的好友ID
	for (size_t i = 0; i < m_vecChatList.size(); i++)
	{
		if (m_vecChatList[i] == fid)
		{
			m_vecChatList.erase(m_vecChatList.begin() + i);
			break;
		}
	}

	//将好友ID存入最后
	m_vecChatList.push_back(fid);

	//清空列表
	m_LastChatList.DeleteAllItems();

	//重新插入
	for (size_t i = 0; i < m_vecChatList.size(); i++)
		m_LastChatList.InsertItem(0, m_vecChatList[i]);

	//保存最近联系人列表到文件
	SaveChatList();
}

void CDlgMessage::SaveChatList()
{
	//写入文件
	//CString sFile = CA2W(m_client->m_szUID, CP_THREAD_ACP);
	CString sFile = CA2T(m_client->m_szUID, CP_THREAD_ACP);
	sFile += L"_lastchat.db";

	CStdioFile file;
	if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{
		//设置字符集
		setlocale(LC_CTYPE, "chs");

		//反向循环写入
		for (size_t i = m_vecChatList.size(); i > 0; i--)
		{
			file.WriteString(m_vecChatList[i - 1] + "\n");
		}

		file.Close();
	}
}

void CDlgMessage::ReadChatList()
{
	//读取文件
	//CString sFile = CA2W(m_client->m_szUID, CP_THREAD_ACP);
	CString sFile = CA2T(m_client->m_szUID, CP_THREAD_ACP);
	sFile += L"_lastchat.db";

	CStdioFile file;
	if (file.Open(sFile, CFile::modeRead))
	{
		setlocale(LC_CTYPE, "chs");
		CString temp;
		//循环写入
		while (file.ReadString(temp))
		{
			m_vecChatList.push_back(temp);
		}
		file.Close();
	}
	//插入列表
	for (size_t i = 0; i < m_vecChatList.size(); i++)
		m_LastChatList.InsertItem(0, m_vecChatList[i]);

}

void CDlgMessage::OnNMDblclkLastchat(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取选中项;
	int nSel = m_LastChatList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("请选中数据");
		return;
	}
	CString fid = m_LastChatList.GetItemText(nSel, 0);

	if (m_client->m_map.find(fid) == m_client->m_map.end())
	{
		//找不到对应窗口就创建
		CDlgOne2One *FrindChat = new CDlgOne2One(m_client, fid);

		FrindChat->Create(IDD_DLG_1TO1);
		FrindChat->ShowWindow(SW_NORMAL);
		//FrindChat->SetWindowTextW(fid);
		FrindChat->SetWindowText(fid);
		m_client->m_map[fid] = FrindChat;

	}
	else
	{
		CDlgOne2One *FrindChat = (CDlgOne2One *)m_client->m_map[fid];
		FrindChat->ShowWindow(SW_NORMAL);
	}

	UpDataChatList(fid);

	*pResult = 0;
}
