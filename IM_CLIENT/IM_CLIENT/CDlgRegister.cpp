// CDlgRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgRegister.h"
#include "afxdialogex.h"


// CDlgRegister �Ի���

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


// CDlgRegister ��Ϣ�������


void CDlgRegister::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	//�����Ϣ�Ƿ�����
	if (m_strUsername.IsEmpty() || m_strPWD1.IsEmpty() || m_strPWD2.IsEmpty())
	{
		MessageBox("������������Ϣ��");
		return;
	}
	//��������Ƿ�һ��
	else if (m_strPWD1 != m_strPWD2)
	{
		MessageBox("�������벻һ�£�");
		return;
	}
	//���ӷ����
	if (!m_client.ConnectServer("192.168.3.184", 1234))
	{
		MessageBox("���ӷ�����ʧ�ܣ�");
		return;
	}


	Sleep(1000);

	//ע��;
	CString strSend = m_strUsername;
	strSend += ":" + m_strPWD1;
	//CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(strSend.GetBuffer(), CP_THREAD_ACP);

	m_client.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1);


	//�ȴ�ע����;
	char *ret = m_client.Recv();

	if (ret == nullptr)
	{
		MessageBox("ע��ʧ�ܣ������ԣ�");
		return;
	}

	else
	{
		int num = atoi(ret);
		CString regok;
		regok.Format("ע��ɹ���\n���μ�ע����Ϣ\n���IDΪ:%d", num);
		MessageBox(regok);
	}

	m_client.Close();

	EndDialog(0);

	return;

}
