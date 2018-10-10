#pragma once
#include "CMyList.h"
#include "afxwin.h"
#include"CClientSocket.h"


// CDlgGroupChat �Ի���

class CDlgGroupChat : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGroupChat)

public:
	CDlgGroupChat(CClientSocket * client, CString &gid, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGroupChat();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUPCHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_gChatList;//Ⱥ��Ա�б�


	CEdit m_ceRecv;
	CString m_strGRecv;
	CString m_strGSend;


	CString m_gid;				//ȺID
	CClientSocket * m_client;

	void UpdataGroupMember();			//����Ⱥ��Ա
	void RecvMemberList(char * uid);	//����Ⱥ��Ա

	afx_msg void OnBnClickedGsend();	//����
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);	//�Ҽ��˵�
	afx_msg void On32771_AddFrined();	//��Ӻ���
	afx_msg void On32772_ShowInfo();	//�鿴����

	virtual void OnOK();
};
