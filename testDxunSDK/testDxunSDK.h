
// testDxunSDK.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestDxunSDKApp:
// �йش����ʵ�֣������ testDxunSDK.cpp
//

class CtestDxunSDKApp : public CWinAppEx
{
public:
	CtestDxunSDKApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestDxunSDKApp theApp;