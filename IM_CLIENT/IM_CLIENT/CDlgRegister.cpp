// CDlgRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgRegister.h"
#include "afxdialogex.h"


// CDlgRegister 对话框

IMPLEMENT_DYNAMIC(CDlgRegister, CDialogEx)

CDlgRegister::CDlgRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_REG, pParent)
	, m_strUsername(_T(""))
	, m_strPWD1(_T(""))
	, m_strPWD2(_T(""))
{

}

CDlgRegister::~CDlgRegister()
{
}

void CDlgRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUsername);
	DDX_Text(pDX, IDC_EDIT2, m_strPWD1);
	DDX_Text(pDX, IDC_EDIT3, m_strPWD2);
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRegister::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgRegister 消息处理程序


void CDlgRegister::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	//检测信息是否完整
	if (m_strUsername.IsEmpty() || m_strPWD1.IsEmpty() || m_strPWD2.IsEmpty())
	{
		MessageBox("请输入完整信息！");
		return;
	}
	//检测密码是否一致
	else if (m_strPWD1 != m_strPWD2)
	{
		MessageBox("输入密码不一致！");
		return;
	}
	//连接服务端
	if (!m_client.ConnectServer("192.168.3.184", 1234))
	{
		MessageBox("连接服务器失败！");
		return;
	}


	Sleep(1000);

	//注册;
	CString strSend = m_strUsername;
	strSend += ":" + m_strPWD1;
	//CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(strSend.GetBuffer(), CP_THREAD_ACP);

	m_client.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1);


	//等待注册结果;
	char *ret = m_client.Recv();

	if (ret == nullptr)
	{
		MessageBox("注册失败！请重试！");
		return;
	}

	else
	{
		int num = atoi(ret);
		CString regok;
		regok.Format("注册成功！\n请牢记注册信息\n你的ID为:%d", num);
		MessageBox(regok);
	}

	m_client.Close();

	EndDialog(0);

	return;

}
