﻿// MD_Calendar.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_webnautes.h"
#include "MD_Calendar.h"
#include "afxdialogex.h"
#include "MFC_webnautesDlg.h"

// MD_Calendar 대화 상자

IMPLEMENT_DYNAMIC(MD_Calendar, CDialogEx)

MD_Calendar::MD_Calendar(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_time(COleDateTime::GetCurrentTime())
{

}

MD_Calendar::~MD_Calendar()
{
}

void MD_Calendar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR1, m_time);
}


BEGIN_MESSAGE_MAP(MD_Calendar, CDialogEx)
	ON_BN_CLICKED(IDOK, &MD_Calendar::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CheckOpenDir(CString sDirName, CString path)
{
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(sDirName + strFile);
	printf("%s", sDirName);
	if (!bResult)
	{
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOW);
		if (!bResult)
		{
			/*Error*/
			return FALSE;
		}
		return TRUE;
	}
	else {
		ShellExecute(NULL, _T("open"), sDirName, NULL, NULL, SW_SHOW);
	}
	return FALSE;
}

// MD_Calendar 메시지 처리기


void MD_Calendar::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
	UpdateData();
	extern CString selFolderDate;
	extern CString CapPath;
	selFolderDate.Format(_T("%04d%02d%02d"), m_time.GetYear(), m_time.GetMonth(), m_time.GetDay());
	CString path = CapPath + "\\" + selFolderDate;
	//AfxMessageBox(path);
	CheckOpenDir(path, CapPath);
	UpdateData(false);
}