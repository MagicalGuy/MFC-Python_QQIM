// CDlgMessage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgMessage.h"
#include "CDlgOne2One.h"
#include "afxdialogex.h"


// CDlgMessage �Ի���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//MoveWindow(0, 0, 365, 440, 1);

	m_LastChatList.AddColumn(1, "����", 200);
	CString str = "LastChat";
	m_client->m_map[str] = this;

	//���ļ���ȡ�б�
	ReadChatList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CDlgMessage, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LASTCHAT, &CDlgMessage::OnNMDblclkLastchat)
END_MESSAGE_MAP()


// CDlgMessage ��Ϣ�������


void CDlgMessage::UpDataChatList(CString fid)
{
	//ɾ���б����Ѵ��ڵĺ���ID
	for (size_t i = 0; i < m_vecChatList.size(); i++)
	{
		if (m_vecChatList[i] == fid)
		{
			m_vecChatList.erase(m_vecChatList.begin() + i);
			break;
		}
	}

	//������ID�������
	m_vecChatList.push_back(fid);

	//����б�
	m_LastChatList.DeleteAllItems();

	//���²���
	for (size_t i = 0; i < m_vecChatList.size(); i++)
		m_LastChatList.InsertItem(0, m_vecChatList[i]);

	//���������ϵ���б��ļ�
	SaveChatList();
}

void CDlgMessage::SaveChatList()
{
	//д���ļ�
	//CString sFile = CA2W(m_client->m_szUID, CP_THREAD_ACP);
	CString sFile = CA2T(m_client->m_szUID, CP_THREAD_ACP);
	sFile += L"_lastchat.db";

	CStdioFile file;
	if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{
		//�����ַ���
		setlocale(LC_CTYPE, "chs");

		//����ѭ��д��
		for (size_t i = m_vecChatList.size(); i > 0; i--)
		{
			file.WriteString(m_vecChatList[i - 1] + "\n");
		}

		file.Close();
	}
}

void CDlgMessage::ReadChatList()
{
	//��ȡ�ļ�
	//CString sFile = CA2W(m_client->m_szUID, CP_THREAD_ACP);
	CString sFile = CA2T(m_client->m_szUID, CP_THREAD_ACP);
	sFile += L"_lastchat.db";

	CStdioFile file;
	if (file.Open(sFile, CFile::modeRead))
	{
		setlocale(LC_CTYPE, "chs");
		CString temp;
		//ѭ��д��
		while (file.ReadString(temp))
		{
			m_vecChatList.push_back(temp);
		}
		file.Close();
	}
	//�����б�
	for (size_t i = 0; i < m_vecChatList.size(); i++)
		m_LastChatList.InsertItem(0, m_vecChatList[i]);

}

void CDlgMessage::OnNMDblclkLastchat(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡѡ����;
	int nSel = m_LastChatList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}
	CString fid = m_LastChatList.GetItemText(nSel, 0);

	if (m_client->m_map.find(fid) == m_client->m_map.end())
	{
		//�Ҳ�����Ӧ���ھʹ���
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
