// CDlgUserInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgUserInfo.h"
#include "afxdialogex.h"


// CDlgUserInfo 对话框

IMPLEMENT_DYNAMIC(CDlgUserInfo, CDialogEx)


CDlgUserInfo::CDlgUserInfo(CClientSocket * client, CString uid, CString uname, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USERINFO, pParent)
{
	m_client = client;
	m_strUID = uid;
	m_strUName = uname;
}



CDlgUserInfo::~CDlgUserInfo()
{
}

void CDlgUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUID);
	DDX_Text(pDX, IDC_EDIT2, m_strUName);
}


void CDlgUserInfo::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_strUID == m_client->m_szUID)
	{
		MessageBox("不能添加自己");
		return;
	}
	//CStringA str = CW2A(m_strUID.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(m_strUID.GetBuffer(), CP_THREAD_ACP);
	m_client->Send(ADDFRIEND, str.GetBuffer(), 1);
	MessageBox("好友请求已发送！");

	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgUserInfo, CDialogEx)
END_MESSAGE_MAP()


// CDlgUserInfo 消息处理程序
