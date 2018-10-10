// CDlgChatRecord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgChatRecord.h"
#include "afxdialogex.h"
#include <locale>


// CDlgChatRecord �Ի���

IMPLEMENT_DYNAMIC(CDlgChatRecord, CDialogEx)


CDlgChatRecord::CDlgChatRecord(CString fid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATRECORD, pParent)
	, m_strRecordText(_T(""))
{
	m_fid = fid;
}



CDlgChatRecord::~CDlgChatRecord()
{
}

void CDlgChatRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORDTEXT, m_ceRecord);
	DDX_Text(pDX, IDC_RECORDTEXT, m_strRecordText);
}


BOOL CDlgChatRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��д�Ƽ�¼


	//��ȡ�ļ�
	CString sFile = m_fid + ".db";
	CStdioFile file;

	if (file.Open(sFile, CFile::modeRead))
	{
		//���ñ��뼯
		setlocale(LC_CTYPE, "chs");
		CString temp;
		//�����Ϣ
		while (file.ReadString(temp))
		{
			m_strRecordText += temp;
			m_strRecordText += L"\r\n";
		}
		file.Close();
	}
	UpdateData(FALSE);

	//������β��;
	m_ceRecord.LineScroll(m_ceRecord.GetLineCount(), 0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CDlgChatRecord, CDialogEx)
END_MESSAGE_MAP()


// CDlgChatRecord ��Ϣ�������
