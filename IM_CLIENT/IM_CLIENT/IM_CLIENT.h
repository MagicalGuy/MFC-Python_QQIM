
// IM_CLIENT.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIM_CLIENTApp:
// �йش����ʵ�֣������ IM_CLIENT.cpp
//

class CIM_CLIENTApp : public CWinApp
{
public:
	CIM_CLIENTApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIM_CLIENTApp theApp;