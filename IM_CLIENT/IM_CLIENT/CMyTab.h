#pragma once
#include "stdafx.h"

// CMyTab

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);	//选项卡切换;s	
	void MyInserItem(DWORD dwCount, ...);							//插入选项卡标签;	
	void MyInsertChild(DWORD dwCount, ...);							//添加子对话框;	
	DWORD m_dwCount;												//保存子对话框信息;
	CDialogEx* m_pWnd[5];											//子窗口数组;
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	
};


