// MD_CONFIG.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_webnautes.h"
#include "MD_CONFIG.h"
#include "afxdialogex.h"


// MD_CONFIG 대화 상자

IMPLEMENT_DYNAMIC(MD_CONFIG, CDialogEx)

MD_CONFIG::MD_CONFIG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MD_CONFIG::~MD_CONFIG()
{
}

void MD_CONFIG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MD_CONFIG, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &MD_CONFIG::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &MD_CONFIG::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &MD_CONFIG::OnBnClickedOk)
END_MESSAGE_MAP()


// MD_CONFIG 메시지 처리기

void MD_CONFIG::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);				// 파일경로 읽어오기

	extern CString CapPath;
	CapPath.Format(_T("%s"), szBuffer);

	// Write 
	WritePrivateProfileString(_T("MotionDetector"), _T("CapPath"), szBuffer, _T(".\\semiMD.ini"));

	AfxMessageBox(CapPath);
}

void MD_CONFIG::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PortText = _T("");

	GetDlgItemText(IDC_EDIT1, PortText);
}

void MD_CONFIG::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
