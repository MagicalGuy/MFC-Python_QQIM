#pragma once
#include "CClientSocket.h"

// CDlgRegister �Ի���

class CDlgRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegister)

public:
	CDlgRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRegister();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CClientSocket m_client;
	CString m_strUsername;
	CString m_strPWD1;
	CString m_strPWD2;
	afx_msg void OnBnClickedButton1();
};
