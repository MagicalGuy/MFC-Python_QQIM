#pragma once
#include "CClientSocket.h"
#include "CMyList.h"

// CDlgFriend �Ի���

class CDlgFriend : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFriend)

public:
	CDlgFriend(CClientSocket * client, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFriend();


	CClientSocket * m_client;
	afx_msg void OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult);
	void  UpdataFriend();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FRIEND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CMyList m_FList;

	afx_msg void OnNMRClickFresh(NMHDR *pNMHDR, LRESULT *pResult);//�Ҽ��˵�
	afx_msg void On32775_FreshInfo();		//ˢ��

};
