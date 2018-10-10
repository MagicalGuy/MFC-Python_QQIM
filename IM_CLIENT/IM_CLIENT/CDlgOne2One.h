#pragma once
#include "CClientSocket.h"
#include "afxwin.h"

// CDlgOne2One �Ի���

class CDlgOne2One : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOne2One)

public:
	CDlgOne2One(CClientSocket * client, CString &fid, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgOne2One();

	CClientSocket * m_client;


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_1TO1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnCancel();							//����ESC��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	CString m_fid;
	CEdit m_ceFRecv;
	CString m_strFSend;
	CString m_strFRecv;

	afx_msg void OnBnClickedFsend();		//������Ϣ
	afx_msg void OnBnClickedChatrecord();	//�鿴���������¼
	afx_msg void OnBnClickedSendfile();		//�����ļ�
	afx_msg void OnBnClickedVideo();		//��Ƶ����
	virtual void OnOK();
	afx_msg void OnBnClickedChatrecord1();//�鿴�Ƽ�¼

};
