#pragma once
#include "afxwin.h"


// CDlgChatRecord �Ի���

class CDlgChatRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChatRecord)

public:
	//CDlgChatRecord(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgChatRecord(CString fid, CWnd* pParent = NULL);
	virtual ~CDlgChatRecord();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATRECORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ceRecord;//�����¼�ؼ�
	CString m_strRecordText; //�󶨱���
	CString m_fid;				//����ID
};
