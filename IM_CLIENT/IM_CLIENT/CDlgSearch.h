#pragma once
#include "CMyList.h"
#include "CClientSocket.h"

// CDlgSearch �Ի���

class CDlgSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSearch)

public:
	CDlgSearch(CClientSocket * client, bool isuid, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSearch();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnCancel();				//����ESC
	DECLARE_MESSAGE_MAP()
public:
	CString m_strid;
	CMyList m_ListSearch;

	CClientSocket * m_client;
	bool m_bIsUid;						  //�Ƿ������û�ID

	afx_msg void OnBnClickedOk();			//���ȷ��
	afx_msg void OnNMRClickSearchlist(NMHDR *pNMHDR, LRESULT *pResult);//�Ҽ��˵�
	afx_msg void On32771_AddFriend();		//��Ӻ���
	afx_msg void On32772_ShowUserInfo();	//�鿴����

};
