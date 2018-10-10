// CDlgOne2One.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgOne2One.h"
#include "CDlgChatRecord.h"
#include "CDlgSendFile.h"
#include "afxdialogex.h"
//#include "Python.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  "sys\types.h"
#include  <sys\stat.h>
#include <locale>

//
//#if defined(_DEBUG)
//#pragma comment(lib,"python36_d.lib")
//#elif defined(Py_LIMITED_API)
//#pragma comment(lib,"python3.lib")
//#else
//#pragma comment(lib,"python36.lib")
//#endif



// CDlgOne2One �Ի���

IMPLEMENT_DYNAMIC(CDlgOne2One, CDialogEx)


CDlgOne2One::CDlgOne2One(CClientSocket * client, CString &fid, CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_DLG_1TO1, pParent)
	, m_strFRecv(_T(""))
	, m_strFSend(_T(""))
{
	m_fid = fid;
	m_client = client;
}


CDlgOne2One::~CDlgOne2One(){}


BEGIN_MESSAGE_MAP(CDlgOne2One, CDialogEx)

	ON_BN_CLICKED(IDC_FSEND, &CDlgOne2One::OnBnClickedFsend)//������Ϣ
	ON_BN_CLICKED(IDC_CHATRECORD, &CDlgOne2One::OnBnClickedChatrecord)//�鿴�����¼
	ON_BN_CLICKED(IDC_SENDFILE, &CDlgOne2One::OnBnClickedSendfile)//�����ļ�
	ON_BN_CLICKED(IDC_VIDEO, &CDlgOne2One::OnBnClickedVideo)//��Ƶ����

	ON_BN_CLICKED(IDC_CHATRECORD1, &CDlgOne2One::OnBnClickedChatrecord1)//�Ƽ�¼
END_MESSAGE_MAP()


// CDlgOne2One ��Ϣ�������



void CDlgOne2One::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FRECV, m_ceFRecv);
	DDX_Text(pDX, IDC_EDIT_FSEND, m_strFSend);
	DDX_Text(pDX, IDC_EDIT_FRECV, m_strFRecv);
}


void CDlgOne2One::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
}

BOOL CDlgOne2One::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//if (pMsg->message == WM_KEYDOWN)
	//{
	//	switch (pMsg->wParam)
	//	{
	//	case VK_RETURN:
	//		OnBnClickedFsend();
	//		break;
	//	default:
	//		break;
	//	}
	//}

	if (pMsg->message == WM_KEYDOWN)
	{
		short int statusCtrl = GetKeyState(VK_CONTROL);
		short int statusX = GetKeyState(VK_RETURN);
		if ((statusCtrl & 0x80) == 0x80 && (statusX & 0x80) == 0x80)
		{
			OnBnClickedFsend();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgOne2One::OnBnClickedFsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	if (m_strFSend.IsEmpty())
	{
		MessageBox("��������Ϣ");
		return;
	}
	//CStringA str = CW2A(m_strFSend.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(m_strFSend.GetBuffer(), CP_THREAD_ACP);
	//DWORD fid = _wtoi(m_fid.GetBuffer());
	DWORD fid = _ttoi(m_fid.GetBuffer());
	m_client->Send(ONE2ONE, str.GetBuffer(), fid);

	//���������¼;

	CString ChatRecord;

	ChatRecord += m_client->m_szName;
	ChatRecord += L"    ";

	//�ͻ���ʱ��;
	COleDateTime time = COleDateTime::GetCurrentTime();
	CString tm = time.Format("%Y-%m-%d %H:%M:%S");

	ChatRecord += tm;
	ChatRecord += L"\r\n";
	ChatRecord += m_strFSend;
	ChatRecord += L"\r\n";

	m_strFRecv += ChatRecord;

	//д���ļ�
	CString sFile = m_fid + ".db";
	CStdioFile file;
	if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{
		setlocale(LC_CTYPE, "chs");
		//�ƶ��ļ�ָ�뵽ĩβ;
		file.SeekToEnd();
		file.WriteString(ChatRecord);
		file.Close();
	}

	m_strFSend = L"";

	UpdateData(FALSE);

}

void CDlgOne2One::OnBnClickedChatrecord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgChatRecord *chatrecor = new CDlgChatRecord(m_fid);
	chatrecor->Create(IDD_CHATRECORD);
	chatrecor->ShowWindow(SW_NORMAL);

}

void CDlgOne2One::OnBnClickedSendfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDlgSendFile *SendFile = new CDlgSendFile(m_client, m_fid, false);
	SendFile->Create(IDD_SENDFILE);
	SendFile->ShowWindow(SW_NORMAL);
	CString str = "FileTrans";//��Ϣ����
	m_client->m_map[str] = SendFile;
}

void CDlgOne2One::OnBnClickedVideo()
{
	int state = system("python main.py");
	if (state == -1) {
		MessageBox("��ȷ�ϻ����Ѿ���������޷���������ϵ�����ߣ�");
	}

}


//�س�����
void CDlgOne2One::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	if (GetDlgItem(IDC_EDIT_FSEND) == GetFocus())
	{//������򡣡���  
		UpdateData(true);
		m_strFSend += "\r\n";
		UpdateData(false);
	}

	CDialogEx::OnOK();
}


void CDlgOne2One::OnBnClickedChatrecord1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_client->Send(MSGRECORD, NULL,NULL);

	// TODO: �ڴ���������������
	CDlgChatRecord * chatrecor = new CDlgChatRecord(m_fid);

	//SearchUser->DoModal();	

	chatrecor->Create(IDD_CHATRECORD);
	chatrecor->ShowWindow(SW_NORMAL);
	//chatrecor->m_strRecordText = "";
	//UpdateData(false);

	CString su = "ChatRecord";
	m_client->m_map[su] = chatrecor;

}
