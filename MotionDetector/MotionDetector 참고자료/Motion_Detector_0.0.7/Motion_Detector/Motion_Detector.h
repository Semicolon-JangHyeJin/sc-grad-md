
// Motion_Detector.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMotion_DetectorApp:
// �� Ŭ������ ������ ���ؼ��� Motion_Detector.cpp�� �����Ͻʽÿ�.
//

class CMotion_DetectorApp : public CWinApp
{
public:
	CMotion_DetectorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMotion_DetectorApp theApp;