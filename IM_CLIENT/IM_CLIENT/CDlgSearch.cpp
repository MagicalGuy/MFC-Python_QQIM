// CDlgSearch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgSearch.h"
#include "CDlgUserInfo.h"
#include "afxdialogex.h"


// CDlgSearch �Ի���

IMPLEMENT_DYNAMIC(CDlgSearch, CDialogEx)


CDlgSearch::CDlgSearch(CClientSocket * client, bool isuid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEARCH, pParent)
	, m_strid(_T(""))
{
	m_client = client;
	m_bIsUid = isuid;
}



CDlgSearch::~CDlgSearch()
{
}

void CDlgSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strid);
	DDX_Control(pDX, IDC_SEARCHLIST, m_ListSearch);
}


void CDlgSearch::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_bIsUid)
	{
		CString su = "SearchUser";
		m_client->m_map.erase(su);
	}
	else
	{
		CString sg = "SearchGroup";
		m_client->m_map.erase(sg);
	}
	CDialogEx::OnCancel();
}

void CDlgSearch::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	//�����û�
	if (m_bIsUid)
	{
		if (m_strid == m_client->m_szUID)
		{
			MessageBox("���������Լ�");
			return;
		}

		//CStringA str = CW2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		CStringA str = CT2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		m_client->Send(SEARCHUSER, str.GetBuffer(), 1);
	}
	else
	{
		//CStringA str = CW2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		CStringA str = CT2A(m_strid.GetBuffer(), CP_THREAD_ACP);
		m_client->Send(SEARCHGROUP, str.GetBuffer(), 1);
	}

	//CDialogEx::OnOK();
}

void CDlgSearch::OnNMRClickSearchlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	//pMenu->LoadMenuW(IDR_MENU1);
	pMenu->LoadMenu(IDR_MENU1);

	CMenu* pSub = NULL;

	//�ж����������û�����Ⱥ
	if (m_bIsUid)
	{
		pSub = pMenu->GetSubMenu(0);
	}
	else
	{
		pSub = pMenu->GetSubMenu(1);
	}
	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}

void CDlgSearch::On32771_AddFriend()
{
	// TODO: �ڴ���������������
	int nSel = m_ListSearch.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}
	CString uid = m_ListSearch.GetItemText(nSel, 0);
	if (uid == m_client->m_szUID)
	{
		MessageBox("��������Լ�");
		return;
	}

	//CStringA str = CW2A(uid.GetBuffer(), CP_THREAD_ACP);
	CStringA str = CT2A(uid.GetBuffer(), CP_THREAD_ACP);
	m_client->Send(ADDFRIEND, str.GetBuffer(), 1);
}

void CDlgSearch::On32772_ShowUserInfo()
{
	// TODO: �ڴ���������������
	//��ȡ��Ϣ
	int nSel = m_ListSearch.GetSelectionMark();
	CString uid = m_ListSearch.GetItemText(nSel, 0);
	CString uname = m_ListSearch.GetItemText(nSel, 1);

	//��ʾ��Ϣ
	CDlgUserInfo *UserInfo = new CDlgUserInfo(m_client, uid, uname);
	UserInfo->Create(IDD_USERINFO);
	UserInfo->ShowWindow(SW_NORMAL);
}

BEGIN_MESSAGE_MAP(CDlgSearch, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSearch::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_SEARCHLIST, &CDlgSearch::OnNMRClickSearchlist)
	ON_COMMAND(ID_32771, &CDlgSearch::On32771_AddFriend)
	ON_COMMAND(ID_32772, &CDlgSearch::On32772_ShowUserInfo)
END_MESSAGE_MAP()


// CDlgSearch ��Ϣ�������
