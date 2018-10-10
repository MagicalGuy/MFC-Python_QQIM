// CDlgGroupChat.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgGroupChat.h"
#include "afxdialogex.h"


// CDlgGroupChat �Ի���

IMPLEMENT_DYNAMIC(CDlgGroupChat, CDialogEx)


CDlgGroupChat::CDlgGroupChat(CClientSocket * client, CString &gid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GROUPCHAT, pParent)
	, m_strGRecv(_T(""))
	, m_strGSend(_T(""))
{
	m_client = client;
	m_gid = gid;
}



CDlgGroupChat::~CDlgGroupChat()
{
}

void CDlgGroupChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GMEMBER, m_gChatList);
	DDX_Control(pDX, IDC_EDIT_GRECV, m_ceRecv);
	DDX_Text(pDX, IDC_EDIT_GRECV, m_strGRecv);
	DDX_Text(pDX, IDC_EDIT_GSEND, m_strGSend);
	DDV_MaxChars(pDX, m_strGSend, 1024);
}


BOOL CDlgGroupChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_gChatList.AddColumn(1,"Ⱥ��Ա", 200);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgGroupChat::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//if (pMsg->message == WM_KEYDOWN)
	//{
	//	switch (pMsg->wParam)
	//	{
	//	case VK_RETURN:
	//		OnBnClickedGsend();
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
			OnBnClickedGsend();
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgGroupChat::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
}

void CDlgGroupChat::UpdataGroupMember()
{
	//����Ⱥ��Ա�б�;	
	m_gChatList.DeleteAllItems();
	//CStringA str = CW2A(m_gid.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(m_gid.GetBuffer(), CP_THREAD_ACP);
	m_client->Send(UPDATEGROUPMEMBER, str.GetBuffer(), str.GetLength());
}

void CDlgGroupChat::RecvMemberList(char * uid)
{
	if (strcmp(uid, "~~~end~~~"))
	{
		CString str(uid);
		m_gChatList.AddItem(1, str);
	}
}

void CDlgGroupChat::OnBnClickedGsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strGSend.IsEmpty())
	{
		MessageBox("��������Ϣ");
		return;
	}
	//CStringA str = CW2A(m_strGSend.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(m_strGSend.GetBuffer(), CP_THREAD_ACP);

	//DWORD gid = _wtoi(m_gid.GetBuffer());
	DWORD gid = _ttoi(m_gid.GetBuffer());
	m_client->Send(ONE2GROUP, str.GetBuffer(), gid);

	m_strGSend = "";
	UpdateData(FALSE);
}

void CDlgGroupChat::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	//pMenu->LoadMenuW(IDR_MENU1);
	pMenu->LoadMenu(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(0);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

void CDlgGroupChat::On32771_AddFrined()
{
	// TODO: �ڴ���������������
	//��ȡѡ����;
	int nSel = m_gChatList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}
	CString uid = m_gChatList.GetItemText(nSel, 0);
	//����Ƿ�����Լ�
	if (uid == m_client->m_szUID)
	{
		MessageBox("��������Լ�");
		return;
	}

	//CStringA str = CW2A(uid.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(uid.GetBuffer(), CP_THREAD_ACP);

	m_client->Send(ADDFRIEND, str.GetBuffer(), 1);
}

void CDlgGroupChat::On32772_ShowInfo()
{
	// TODO: �ڴ���������������
	//��ȡѡ����;
	int nSel = m_gChatList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}
	CString uid = m_gChatList.GetItemText(nSel, 0);

	//�Թ�
	MessageBox(uid);
}

BEGIN_MESSAGE_MAP(CDlgGroupChat, CDialogEx)
	ON_BN_CLICKED(IDC_GSEND, &CDlgGroupChat::OnBnClickedGsend)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_GMEMBER, &CDlgGroupChat::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CDlgGroupChat::On32771_AddFrined)
	ON_COMMAND(ID_32772, &CDlgGroupChat::On32772_ShowInfo)


END_MESSAGE_MAP()


// CDlgGroupChat ��Ϣ�������


void CDlgGroupChat::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���


		if (GetDlgItem(IDC_EDIT_GSEND) == GetFocus())
		{//������򡣡���  
			UpdateData(true);
			m_strGSend += "\r\n";
			UpdateData(false);
		}

	CDialogEx::OnOK();
}
