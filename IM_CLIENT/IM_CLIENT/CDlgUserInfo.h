#pragma once
#include "CClientSocket.h"

// CDlgUserInfo �Ի���

class CDlgUserInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserInfo)

public:
	CDlgUserInfo(CClientSocket * client, CString uid, CString uname, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUserInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();		//��дOK
	DECLARE_MESSAGE_MAP()
public:
	CString m_strUID;
	CString m_strUName;

	CClientSocket * m_client;
};
