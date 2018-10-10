// CDlgFriend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgFriend.h"
#include"CDlgOne2One.h"
#include "CDlgMessage.h"
#include "afxdialogex.h"


// CDlgFriend �Ի���

IMPLEMENT_DYNAMIC(CDlgFriend, CDialogEx)

CDlgFriend::CDlgFriend(CClientSocket * client, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FRIEND, pParent)
{
	m_client = client;
}



CDlgFriend::~CDlgFriend(){}


BEGIN_MESSAGE_MAP(CDlgFriend, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FRIEND, &CDlgFriend::OnNMDblclkListFriend)

	ON_NOTIFY(NM_RCLICK, IDC_LIST_FRIEND, &CDlgFriend::OnNMRClickFresh)

	ON_COMMAND(ID_32775, &CDlgFriend::On32775_FreshInfo)

END_MESSAGE_MAP()


// CDlgFriend ��Ϣ�������



void CDlgFriend::OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡѡ����;
	int nSel = m_FList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}

	CString fid = m_FList.GetItemText(nSel, 0);

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
		//��ʾ���촰��
		CDlgOne2One *FrindChat = (CDlgOne2One *)m_client->m_map[fid];
		FrindChat->ShowWindow(SW_NORMAL);
	}

	//������������б�;
	CString str = "LastChat";
	CDlgMessage *lastmessage = (CDlgMessage *)m_client->m_map[str];
	lastmessage->UpDataChatList(fid);

	*pResult = 0;
}

void CDlgFriend::UpdataFriend()
{
	//��������б�;	
	m_client->Send(UPDATEFRIEND, m_client->m_szUID, strlen(m_client->m_szUID));
	char *ret = m_client->Recv();
	//ѭ������
	while (strcmp(ret, "~~~end~~~"))
	{
		//CString str = CA2W(ret, CP_THREAD_ACP);
		CString str = CA2A(ret, CP_THREAD_ACP);
		m_FList.AddItem(1, str);
		ret = m_client->Recv();
	}
}

void CDlgFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_FList);
}


BOOL CDlgFriend::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//MoveWindow(0, 0, 365, 440, 1);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_FList.AddColumn(1, L"����", 250);//�б�ؼ�����report��Ч

	UpdataFriend();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



void CDlgFriend::OnNMRClickFresh(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	//pMenu->LoadMenuW(IDR_MENU1);
	pMenu->LoadMenu(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(1);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_BOTTOMALIGN, point.x, point.y, this, NULL);
}

void CDlgFriend::On32775_FreshInfo()
{
	//m_FList.DeleteAllItems();
	//UpdataFriend();
	MessageBox("��������bug,�д��Ľ�");
}