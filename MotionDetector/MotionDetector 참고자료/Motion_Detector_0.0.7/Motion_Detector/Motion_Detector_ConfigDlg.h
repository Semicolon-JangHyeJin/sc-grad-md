#pragma once
#include "resource.h"

#define UM_SAVE_POSTMESSAGE WM_USER + 1

#define INI_CATEGORY		L"Motion_Detector"
#define INI_PARAMETER_PORT	L"Port"
#define INI_PARAMETER_PATH	L"Path"
#define INI_PARAMETER_DUEDATE	L"Due_Date"
#define INI_PARAMETER_COMBOSEL	L"Combo_Sel"

// Motion_Detector_ConfigDlg 대화 상자입니다.

class Motion_Detector_ConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(Motion_Detector_ConfigDlg)

public:
	Motion_Detector_ConfigDlg(Motion_Detector_ConfigDlg*& pOwnObjPtr);   // 표준 생성자입니다.
	virtual ~Motion_Detector_ConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTION_DETECTOR_CONFIG };

	BOOL Create(CWnd* pParentWnd = NULL);

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	Motion_Detector_ConfigDlg*&	mpOwnObjPtr;

	CComboBox mComboBox;

	CString	mPath;
	TCHAR iniPath[MAX_PATH];
	UINT mPort;
	UINT mComboSel;
	UINT mComboList2Date[14];
	UINT mDueDate;
	UINT mToday;
	

	
public:
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnEnChangePortEdit();
	afx_msg void OnCbnSelchangeSaveDateCombo();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};
