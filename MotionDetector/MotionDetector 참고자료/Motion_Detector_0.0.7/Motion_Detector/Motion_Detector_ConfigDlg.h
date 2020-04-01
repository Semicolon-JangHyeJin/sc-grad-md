#pragma once
#include "resource.h"

#define UM_SAVE_POSTMESSAGE WM_USER + 1

#define INI_CATEGORY		L"Motion_Detector"
#define INI_PARAMETER_PORT	L"Port"
#define INI_PARAMETER_PATH	L"Path"
#define INI_PARAMETER_DUEDATE	L"Due_Date"
#define INI_PARAMETER_COMBOSEL	L"Combo_Sel"

// Motion_Detector_ConfigDlg ��ȭ �����Դϴ�.

class Motion_Detector_ConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(Motion_Detector_ConfigDlg)

public:
	Motion_Detector_ConfigDlg(Motion_Detector_ConfigDlg*& pOwnObjPtr);   // ǥ�� �������Դϴ�.
	virtual ~Motion_Detector_ConfigDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MOTION_DETECTOR_CONFIG };

	BOOL Create(CWnd* pParentWnd = NULL);

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	// ������ �޽��� �� �Լ�
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
