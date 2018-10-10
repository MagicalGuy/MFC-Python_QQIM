// CDlgChatRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgChatRecord.h"
#include "afxdialogex.h"
#include <locale>


// CDlgChatRecord 对话框

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

	// TODO:  在此添加额外的初始化

	//抄写云记录


	//读取文件
	CString sFile = m_fid + ".db";
	CStdioFile file;

	if (file.Open(sFile, CFile::modeRead))
	{
		//设置编码集
		setlocale(LC_CTYPE, "chs");
		CString temp;
		//输出消息
		while (file.ReadString(temp))
		{
			m_strRecordText += temp;
			m_strRecordText += L"\r\n";
		}
		file.Close();
	}
	UpdateData(FALSE);

	//滚动到尾行;
	m_ceRecord.LineScroll(m_ceRecord.GetLineCount(), 0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CDlgChatRecord, CDialogEx)
END_MESSAGE_MAP()


// CDlgChatRecord 消息处理程序
