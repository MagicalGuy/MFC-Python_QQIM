// CDlgGroup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgGroup.h"
#include"CDlgGroupChat.h"
#include "afxdialogex.h"


// CDlgGroup �Ի���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_GList.AddColumn(1, L"Ⱥ��", 200);

	//����Ⱥ�б�
	UpdataGroup();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgGroup::UpdataGroup()
{
	//����Ⱥ�б�;	

	m_client->Send(UPDATEGROUP, m_client->m_szUID, strlen(m_client->m_szUID));

	char *ret = m_client->Recv();

	//ѭ������
	while (strcmp(ret, "~~~end~~~"))
	{
		//CString str = CA2W(ret, CP_THREAD_ACP);
		CString str = CA2T(ret, CP_THREAD_ACP);
		m_GList.AddItem(1, str);
		ret = m_client->Recv();
	}
}


//˫����Ⱥ��
void CDlgGroup::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡѡ����;
	int nSel = m_GList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}
	CString gid = m_GList.GetItemText(nSel, 0);

	if (m_client->m_map.find(gid) == m_client->m_map.end())
	{
		//�Ҳ�����Ӧ���ھʹ���
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
		//��ʾ���촰��
		CDlgGroupChat *GroupChat = (CDlgGroupChat *)m_client->m_map[gid];
		GroupChat->ShowWindow(SW_NORMAL);
		GroupChat->UpdataGroupMember();
	}

	*pResult = 0;
}

BEGIN_MESSAGE_MAP(CDlgGroup, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUP, &CDlgGroup::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgGroup ��Ϣ�������
