// IM_MAIN.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "IM_MAIN.h"
#include"CDlgMessage.h"
#include"CDlgFriend.h"
#include"CDlgGroup.h"
#include "CDlgGroupChat.h"
#include "CDlgOne2One.h"
#include "CDlgSearch.h"
#include "CDlgSendFile.h"
#include "CDlgChatRecord.h"
#include"xxtea.h"
#include "afxdialogex.h"


// IM_MAIN �Ի���

IMPLEMENT_DYNAMIC(IM_MAIN, CDialogEx)


IM_MAIN::IM_MAIN(CClientSocket * client, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHAT_MAIN, pParent)
{
	m_client = client;
}


IM_MAIN::~IM_MAIN(){}

void IM_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_TAB, m_tabctrl);
	DDX_Control(pDX, IDC_MYIMG, m_staMyImg);
	DDX_Control(pDX, IDC_MYNAME, m_staMyName);
}

BEGIN_MESSAGE_MAP(IM_MAIN, CDialogEx)
	ON_BN_CLICKED(IDC_ADD, &IM_MAIN::OnBnClickedAdd)
	ON_COMMAND(ID_32773, &IM_MAIN::On32773SearchUser)
	ON_COMMAND(ID_32774, &IM_MAIN::On32774SearchGroup)
	ON_MESSAGE(WM_MY_MESSAGE, OnMyMessage)

	ON_WM_PAINT()
	ON_BN_DOUBLECLICKED(IDC_ADD, &IM_MAIN::OnBnDoubleclickedAdd)
	ON_STN_CLICKED(IDC_MYIMG, &IM_MAIN::OnStnClickedMyimg)
	
	//ON_COMMAND(ID_32784, &IM_MAIN::On32784_Set)
END_MESSAGE_MAP()


// IM_MAIN ��Ϣ�������


BOOL IM_MAIN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//CString name = CA2W(m_client->m_szName, CP_THREAD_ACP);
	//m_staMyName.SetWindowTextW(name);
	CString name = CA2T(m_client->m_szName, CP_THREAD_ACP);
	m_staMyName.SetWindowText(name);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_tabctrl.MyInserItem(3, "����", "Ⱥ��","�����ϵ");

	//���봰��
	m_tabctrl.MyInsertChild(3,
		new CDlgFriend(m_client, this), IDD_DLG_FRIEND,
		new CDlgGroup(m_client, this), IDD_DLG_GROUP,
		new CDlgMessage(m_client, this), IDD_DLG_MSG);

	//���ñ�ǩ��С
	CSize size;
	size.SetSize(115, 30);
	m_tabctrl.SetItemSize(size);
	//�޸Ĵ�����ʽ
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

	//����ͼ��
	HICON m_hIcon;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//���ô��ڱ���
	//CString str = CA2W(m_client->m_szName, CP_THREAD_ACP);
	//SetWindowTextW(str);
	CString str = CA2T(m_client->m_szName, CP_THREAD_ACP);
	SetWindowText(str);

	//����MAP
	m_client->m_map[str] = this;

	//���������߳�;
	m_hRecv = AfxBeginThread(ThreadRecv, this);

	//��ȡ������Ϣ 
	m_client->Send(MSGOFFLINE, m_client->m_szUID, strlen(m_client->m_szUID));

	//��ȡ�ؼ���С
	m_staMyImg.GetWindowRect(&m_rect);
	//ת������
	ScreenToClient(&m_rect);
	//�ƶ�����
	m_staMyImg.MoveWindow(m_rect.left, m_rect.top, 100, 100, TRUE);

	//����ͷ��
	m_image.Load("HEAD_106.png");

	//����ͼƬ;
	m_hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PNG_HEAD_102));
	if (m_image == NULL)
	{
		m_staMyImg.SetBitmap(m_hBitmap);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



void IM_MAIN::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}

void IM_MAIN::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}


UINT IM_MAIN::ThreadRecv(LPVOID lpParam)
{
	IM_MAIN* ChatMain = (IM_MAIN*)lpParam;

	//ѭ������
	while (ChatMain->m_hRecv)
	{
		//������Ϣ;
		CHATSEND ct = {};
		if (SOCKET_ERROR == recv(ChatMain->m_client->m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
		{
			ChatMain->MessageBox("�����������ӶϿ�");
			::SendMessage(ChatMain->m_hWnd, WM_CLOSE, NULL, NULL);
			return -1;
		}

		//�ַ���Ϣ
		switch (ct.m_type)
		{
		case ADDFRIEND:
			ChatMain->AddFrinend(ct);
			break;
		case ONE2GROUP:
			ChatMain->OneToGroup(ct);
			break;
		case ONE2ONE:
			ChatMain->OneToOne(ct);
			break;
		case UPDATEGROUPMEMBER:
		{
			CString gid(ct.m_content.gml.gid);
			((CDlgGroupChat *)ChatMain->m_client->m_map[gid])->RecvMemberList(ct.m_content.gml.mid);
		}
		break;
		case  SEARCHUSER:
			ChatMain->SearchUser(ct);
			break;
		case SEARCHGROUP:
			ChatMain->SearchGroup(ct);
			break;
		case MSGRECORD:
			ChatMain->ChatRecord(ct);
			break;
		case REPLOGIN:
		{
			ChatMain->MessageBox("���������ͻ��˵�¼");
			::SendMessage(ChatMain->m_hWnd, WM_CLOSE, NULL, NULL);
			return -1;
		}
		case FILETRANS:
			ChatMain->FileTrans(ct);
			break;
		case MSGVIDEO:
			ChatMain->ChatVideo(ct);
			break;
		default:
			break;
		}
	}
	return 0;
}




void IM_MAIN::drawimg()
{
	if (m_image != NULL)
	{
		CWnd *pWnd = NULL;
		pWnd = GetDlgItem(IDC_MYIMG);//��ȡ�ؼ����
		pWnd->GetClientRect(&m_rect);//��ȡ���ָ��ؼ�����Ĵ�С

		CDC *pDc = NULL;
		pDc = pWnd->GetDC();//��ȡpicture��DC
		m_image.Draw(pDc->m_hDC, 0, 0, 90, 90);//��ͼƬ���Ƶ�picture��ʾ��������
		ReleaseDC(pDc);
	}
}

void IM_MAIN::OnBnDoubleclickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void IM_MAIN::OnStnClickedMyimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



void IM_MAIN::AddFrinend(CHATSEND& ct)
{
	if (!strcmp(this->m_client->m_szUID, ct.m_content.adf.szFrd))
	{
		if (ct.m_content.adf.dwAdd == 3)
		{
			//�ܾ�;�޲���

			return;
		}

		if (ct.m_content.adf.dwAdd == 2)
		{
			//��ӳɹ�;���º����б�
			((CDlgFriend *)GetDlgItem(IDD_DLG_FRIEND))->UpdataFriend();
			return;
		}

		if (ct.m_content.adf.dwAdd == 0)
		{
			//���ʧ��
			return;
		}

		CString str;
		str += ct.m_content.adf.szName;
		str += "  ��Ҫ�����Ϊ����\r\n�Ƿ�ͬ�⣿";
		int ret = this->MessageBox(str, "ϵͳ��Ϣ", MB_OKCANCEL);

		//ͬ��
		if (ret == IDOK)
		{
			this->m_client->Send(ADDFRIEND, ct.m_content.adf.szName, 2);
		}
		//�ܾ�
		else
		{
			this->m_client->Send(ADDFRIEND, ct.m_content.adf.szName, 3);
		}
	}
}




void IM_MAIN::OneToGroup(CHATSEND& ct)
{
	CString strGID(ct.m_content.o2n.szGID);

	if (m_client->m_map.find(strGID) == m_client->m_map.end())
	{

		//������Ϣ��������;
		::SendMessage(this->m_hWnd, WM_MY_MESSAGE, NULL, (LPARAM)&ct);

	}
	else
	{
		//���ڴ���ֱ�Ӹ�����Ϣ
		CDlgGroupChat *GroupChat = (CDlgGroupChat *)m_client->m_map[strGID];
		GroupChat->m_strGRecv += L"\r\n";
		GroupChat->m_strGRecv += ct.m_content.o2n.szName;
		GroupChat->m_strGRecv += L"    ";
		GroupChat->m_strGRecv += ct.m_content.o2n.szTime;
		GroupChat->m_strGRecv += L"\r\n";
		GroupChat->m_strGRecv += ct.m_content.o2n.szContent;
		//GroupChat->m_ceRecv.SetWindowTextW(GroupChat->m_strGRecv);
		GroupChat->m_ceRecv.SetWindowText(GroupChat->m_strGRecv);
		GroupChat->m_ceRecv.LineScroll(GroupChat->m_ceRecv.GetLineCount(), 0);

	}
}


void IM_MAIN::OneToOne(CHATSEND& ct)
{
	//CString strFID = CA2W(ct.m_content.o2o.szUid, CP_THREAD_ACP);
	CString strFID = CA2A(ct.m_content.o2o.szUid, CP_THREAD_ACP);
	if (m_client->m_map.find(strFID) == m_client->m_map.end())
	{
		//������Ϣ��������;
		::SendMessage(this->m_hWnd, WM_MY_MESSAGE, NULL, (LPARAM)&ct);

	}
	else
	{
		//���ڴ��ڸ�����Ϣ
		CDlgOne2One *FrindChat = (CDlgOne2One *)m_client->m_map[strFID];

		//����
		string cipherText = ct.m_content.o2o.szContent;
		string key = ct.m_content.o2o.szUid;
		string Content = xxtea_decrypt(cipherText, key);

		//������Ϣ
		CString ChatRecord;
		ChatRecord += ct.m_content.o2o.szUname;
		ChatRecord += L"    ";
		ChatRecord += ct.m_content.o2o.szTime;
		ChatRecord += L"\r\n";
		ChatRecord += Content.c_str();
		ChatRecord += L"\r\n";

		FrindChat->m_strFRecv += ChatRecord;
		//FrindChat->m_ceFRecv.SetWindowTextW(FrindChat->m_strFRecv);
		FrindChat->m_ceFRecv.SetWindowText(FrindChat->m_strFRecv);
		FrindChat->m_ceFRecv.LineScroll(FrindChat->m_ceFRecv.GetLineCount(), 0);


		//д���ļ�
		CString sFile = FrindChat->m_fid + ".db";
		CStdioFile file;
		if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{
			setlocale(LC_CTYPE, "chs");
			//�ƶ��ļ�ָ�뵽ĩβ;
			file.SeekToEnd();
			file.WriteString(ChatRecord);
			file.Close();
		}

		//��������
		//PlaySound(L"msg.wav", NULL, SND_ASYNC);
	}

	//������������б�;
	CString str = "LastChat";
	CDlgMessage *lastmessage = (CDlgMessage *)m_client->m_map[str];
	lastmessage->UpDataChatList(strFID);

}



void IM_MAIN::SearchUser(CHATSEND& ct)
{
	CString su = "SearchUser";
	CDlgSearch * SearchUser = (CDlgSearch *)m_client->m_map[su];
	SearchUser->m_ListSearch.AddColumn(2, "�û�ID", 100, "�û���", 100);
	//CString uid = CA2W(ct.m_content.su.szUID, CP_THREAD_ACP);
	//CString uname = CA2W(ct.m_content.su.szName, CP_THREAD_ACP);
	CString uid = CA2A(ct.m_content.su.szUID, CP_THREAD_ACP);
	CString uname = CA2A(ct.m_content.su.szName, CP_THREAD_ACP);
	SearchUser->m_ListSearch.DeleteAllItems();
	SearchUser->m_ListSearch.AddItem(2, uid, uname);
}

void IM_MAIN::SearchGroup(CHATSEND& ct)
{
	CString sg = "SearchGroup";
	CDlgSearch * SearchUser = (CDlgSearch *)m_client->m_map[sg];
	SearchUser->m_ListSearch.AddColumn(2, "ȺID", 100, "Ⱥ��", 100);
	//CString uid = CA2W(ct.m_content.su.szUID, CP_THREAD_ACP);
	//CString uname = CA2W(ct.m_content.su.szName, CP_THREAD_ACP);
	CString uid = CT2A(ct.m_content.su.szUID, CP_THREAD_ACP);
	CString uname = CT2A(ct.m_content.su.szName, CP_THREAD_ACP);
	SearchUser->m_ListSearch.DeleteAllItems();
	SearchUser->m_ListSearch.AddItem(2, uid, uname);
}




void IM_MAIN::FileTrans(CHATSEND& ct)
{
	//�ļ�����
	if (ct.m_content.file.dwType == 1)
	{
		CStringA str = ct.m_content.file.szUid;
		str += "�������ļ���";
		str += ct.m_content.file.szFileName;
		str += "\r\n�Ƿ���ܣ�";

		//CString message = CA2W(str.GetBuffer(), CP_THREAD_ACP);
		CString message = CA2T(str.GetBuffer(), CP_THREAD_ACP);
		int nRet = MessageBox(message, "�����ļ����գ�", MB_OKCANCEL);

		if (nRet == IDOK)
		{
			CStringA fid = ct.m_content.file.szUid;
			m_client->Send(FILETRANS, fid.GetBuffer(), 2);

			CString str = "FileTrans";
			if (m_client->m_map.find(str) == m_client->m_map.end())
			{
				//������Ϣ��������;
				::SendMessage(this->m_hWnd, WM_MY_MESSAGE, NULL, (LPARAM)&ct);

			}
			else
			{
				CDlgSendFile * SendFile = (CDlgSendFile *)m_client->m_map[str];
				::SendMessage(SendFile->m_hWnd, WM_USER + 101, (WPARAM)&ct, 2);
			}

		}
		else
		{
			CStringA fid = ct.m_content.file.szUid;
			m_client->Send(FILETRANS, fid.GetBuffer(), 3);
		}
	}
	else if (ct.m_content.file.dwType == 2)
	{
		CStringA fid = ct.m_content.file.szUid;

		CString str = "FileTrans";
		if (m_client->m_map.find(str) == m_client->m_map.end())
		{

			//������Ϣ��������;
			::SendMessage(this->m_hWnd, WM_MY_MESSAGE, NULL, (LPARAM)&ct);

		}
		else
		{
			CDlgSendFile * SendFile = (CDlgSendFile *)m_client->m_map[str];
			SendFile->m_frindIP = ct.m_content.file.szFileName;
			::SendMessage(SendFile->m_hWnd, WM_USER + 101, (WPARAM)&ct, 1);
		}
	}
	else if (ct.m_content.file.dwType == 3)
	{
		CStringA fid = ct.m_content.file.szUid;
		fid += "�ܾ������ļ���";
		//CString err = CA2W(fid.GetBuffer(), CP_THREAD_ACP);
		CString err = CA2T(fid.GetBuffer(), CP_THREAD_ACP);
		MessageBox(err);


		CString str = "FileTrans";
		if (m_client->m_map.find(str) == m_client->m_map.end())
		{

		}
		else
		{
			CDlgSendFile * SendFile = (CDlgSendFile *)m_client->m_map[str];
			::SendMessage(SendFile->m_hWnd, WM_CLOSE, NULL, NULL);
			m_client->m_map.erase(str);
		}

	}
	else if (ct.m_content.file.dwType == 4)
	{
		CStringA fid = ct.m_content.file.szUid;
		fid += ct.m_content.file.szFileName;
		//CString err = CA2W(fid.GetBuffer(), CP_THREAD_ACP);
		CString err = CA2T(fid.GetBuffer(), CP_THREAD_ACP);
		MessageBox(err);


		CString str = "FileTrans";
		if (m_client->m_map.find(str) == m_client->m_map.end())
		{

		}
		else
		{
			CDlgSendFile * SendFile = (CDlgSendFile *)m_client->m_map[str];
			::SendMessage(SendFile->m_hWnd, WM_CLOSE, NULL, NULL);
			m_client->m_map.erase(str);
		}

	}
}



void IM_MAIN::ChatVideo(CHATSEND& ct)
{

}

void IM_MAIN::ChatRecord(CHATSEND& ct)
{
	CString mrc = "ChatRecord";
	CDlgChatRecord * ChatRecord = (CDlgChatRecord *)m_client->m_map[mrc];


	//����
	string cipherText = ct.m_content.o2o.szContent;
	string key = ct.m_content.o2o.szUid;
	string Content = xxtea_decrypt(cipherText, key);

	//������Ϣ
	CString ChatRecord1;
	ChatRecord1 += ct.m_content.o2o.szUname;
	ChatRecord1 += L"    ";
	ChatRecord1 += ct.m_content.o2o.szTime;
	ChatRecord1 += L"\r\n";
	ChatRecord1 += Content.c_str();
	ChatRecord1 += L"\r\n";

	ChatRecord->m_strRecordText += ChatRecord1;

	//FrindChat->m_ceFRecv.SetWindowTextW(FrindChat->m_strFRecv);
	ChatRecord->m_ceRecord.SetWindowText(ChatRecord->m_strRecordText);
	ChatRecord->m_ceRecord.LineScroll(ChatRecord->m_ceRecord.GetLineCount(), 0);

}

void IM_MAIN::OnBnClickedAdd()
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

void IM_MAIN::On32773SearchUser()
{
	// TODO: �ڴ���������������
	CDlgSearch * SearchUser = new CDlgSearch(m_client, true);

	//SearchUser->DoModal();	

	SearchUser->Create(IDD_SEARCH);
	SearchUser->ShowWindow(SW_NORMAL);

	CString su = "SearchUser";
	m_client->m_map[su] = SearchUser;
}

void IM_MAIN::On32774SearchGroup()
{
	// TODO: �ڴ���������������
	CDlgSearch * SearchGroup = new CDlgSearch(m_client, false);

	SearchGroup->Create(IDD_SEARCH);
	SearchGroup->ShowWindow(SW_NORMAL);

	CString sg = "SearchGroup";
	m_client->m_map[sg] = SearchGroup;
}


	LRESULT IM_MAIN::OnMyMessage(WPARAM wParam, LPARAM lParam)
	{
		CHATSEND* ct = (CHATSEND*)lParam;

		//1v1��Ϣ
		if (ct->m_type == ONE2ONE)
		{
			//CString strFID = CA2W(ct->m_content.o2o.szUid, CP_THREAD_ACP);
			CString strFID = CA2A(ct->m_content.o2o.szUid, CP_THREAD_ACP);

			CDlgOne2One *FrindChat = new CDlgOne2One(m_client, strFID);

			FrindChat->Create(IDD_DLG_1TO1);
			FrindChat->ShowWindow(SW_NORMAL);
			//FrindChat->SetWindowTextW(strFID);
			FrindChat->SetWindowText(strFID);

			m_client->m_map[strFID] = FrindChat;

			//����
			string cipherText = ct->m_content.o2o.szContent;
			string key = ct->m_content.o2o.szUid;
			string Content = xxtea_decrypt(cipherText, key);

			//��Ϣ����
			CString ChatRecord;
			ChatRecord += ct->m_content.o2o.szUname;
			ChatRecord += L"    ";
			ChatRecord += ct->m_content.o2o.szTime;
			ChatRecord += L"\r\n";
			ChatRecord += Content.c_str();
			ChatRecord += L"\r\n";


			FrindChat->m_strFRecv += ChatRecord;
			//FrindChat->m_ceFRecv.SetWindowTextW(FrindChat->m_strFRecv);
			FrindChat->m_ceFRecv.SetWindowText(FrindChat->m_strFRecv);
			FrindChat->m_ceFRecv.LineScroll(FrindChat->m_ceFRecv.GetLineCount(), 0);

			//д���ļ�
			CString sFile = FrindChat->m_fid + ".db";
			CStdioFile file;
			if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
			{
				setlocale(LC_CTYPE, "chs");
				//�ƶ��ļ�ָ�뵽ĩβ;
				file.SeekToEnd();
				file.WriteString(ChatRecord);
				file.Close();
			}

		}

		//1vN��Ϣ
		if (ct->m_type == ONE2GROUP)
		{
			//CString strGID = CA2W(ct->m_content.o2n.szGID, CP_THREAD_ACP);
			CString strGID = CA2T(ct->m_content.o2n.szGID, CP_THREAD_ACP);
			//�Ҳ�����Ӧ���ھʹ���
			CDlgGroupChat *GroupChat = new CDlgGroupChat(m_client, strGID, this);

			GroupChat->Create(IDD_GROUPCHAT);
			GroupChat->ShowWindow(SW_NORMAL);
			//GroupChat->SetWindowTextW(strGID);
			GroupChat->SetWindowText(strGID);

			m_client->m_map[strGID] = GroupChat;

			//��Ϣ����
			GroupChat->m_strGRecv += L"\r\n";
			GroupChat->m_strGRecv += ct->m_content.o2n.szName;
			GroupChat->m_strGRecv += L"    ";
			GroupChat->m_strGRecv += ct->m_content.o2n.szTime;
			GroupChat->m_strGRecv += L"\r\n";
			GroupChat->m_strGRecv += ct->m_content.o2n.szContent;
			//GroupChat->m_ceRecv.SetWindowTextW(GroupChat->m_strGRecv);
			GroupChat->m_ceRecv.SetWindowText(GroupChat->m_strGRecv);
			GroupChat->m_ceRecv.LineScroll(GroupChat->m_ceRecv.GetLineCount(), 0);
		}
		//if(ct->m_type == MSGRECORD){

		//	CString strRID = CA2T(ct->m_content.mrc.szUid, CP_THREAD_ACP);
		//	//�Ҳ�����Ӧ���ھʹ���
		//	CDlgChatRecord *GroupChat = new CDlgChatRecord(m_client, strRID, this);

		//	GroupChat->Create(IDD_GROUPCHAT);
		//	GroupChat->ShowWindow(SW_NORMAL);
		//	//GroupChat->SetWindowTextW(strGID);
		//	GroupChat->SetWindowText(strGID);

		//	m_client->m_map[strGID] = GroupChat;

		//	CString mrc = "ChatRecord";
		//	CDlgChatRecord * ChatRecord = (CDlgChatRecord *)m_client->m_map[mrc];


		//	//����
		//	string cipherText = ct.m_content.o2o.szContent;
		//	string key = ct.m_content.o2o.szUid;
		//	string Content = xxtea_decrypt(cipherText, key);

		//	//������Ϣ
		//	CString ChatRecord1;
		//	ChatRecord1 += ct.m_content.o2o.szUname;
		//	ChatRecord1 += L"    ";
		//	ChatRecord1 += ct.m_content.o2o.szTime;
		//	ChatRecord1 += L"\r\n";
		//	ChatRecord1 += Content.c_str();
		//	ChatRecord1 += L"\r\n";

		//	ChatRecord->m_strRecordText += ChatRecord1;

		//	//FrindChat->m_ceFRecv.SetWindowTextW(FrindChat->m_strFRecv);
		//	ChatRecord->m_ceRecord.SetWindowText(ChatRecord->m_strRecordText);
		//	ChatRecord->m_ceRecord.LineScroll(ChatRecord->m_ceRecord.GetLineCount(), 0);

		//}

		//�ļ�������Ϣ
		if (ct->m_type == FILETRANS)
		{
			CStringA fid = ct->m_content.file.szUid;
			//CString csfid = CA2W(fid.GetBuffer(), CP_THREAD_ACP);
			CString csfid = CA2T(fid.GetBuffer(), CP_THREAD_ACP);
			CDlgSendFile *SendFile = new CDlgSendFile(m_client, csfid, true);
			SendFile->m_strFileName = ct->m_content.file.szFileName;
			SendFile->Create(IDD_SENDFILE);
			SendFile->ShowWindow(SW_NORMAL);
			CString str = "FileTrans";
			m_client->m_map[str] = SendFile;
		}

		//��Ƶ������Ϣ
		if (ct->m_type == MSGVIDEO)
		{
			////��������
			//CStringA fid = ct->m_content.video.szUid;
			//CString csfid = CA2W(fid.GetBuffer(), CP_THREAD_ACP);
			//string str = "";
			//CChatVideo * ChatVideo = new CChatVideo(str);

			////wParam=0Ϊ�ͻ���
			//if (!wParam)
			//{
			//	ChatVideo->m_videoIP = ct->m_content.video.szContent;
			//}
			////1Ϊ�ͻ���

			//ChatVideo->Create(IDD_VIDEO);
			//ChatVideo->ShowWindow(SW_NORMAL);
		}

		return 0;
	}


	void IM_MAIN::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
						   // TODO: �ڴ˴������Ϣ����������
						   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
		drawimg();
	}

