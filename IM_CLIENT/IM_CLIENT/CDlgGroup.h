#pragma once
#include "CMyList.h"
#include "CClientSocket.h"

// CDlgGroup �Ի���

class CDlgGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGroup)

public:
	//CDlgGroup(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgGroup(CClientSocket * client, CWnd* pParent = NULL);
	virtual ~CDlgGroup();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GROUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_GList;

	CClientSocket * m_client;
	void UpdataGroup();			//����Ⱥ�б�

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
