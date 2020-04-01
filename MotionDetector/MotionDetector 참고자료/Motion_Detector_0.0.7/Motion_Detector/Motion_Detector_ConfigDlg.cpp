// Motion_Detector_ConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Motion_Detector_ConfigDlg.h"
#include "afxdialogex.h"


// Motion_Detector_ConfigDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(Motion_Detector_ConfigDlg, CDialogEx)

Motion_Detector_ConfigDlg::Motion_Detector_ConfigDlg(Motion_Detector_ConfigDlg*& pOwnObjPtr)
	: CDialogEx(Motion_Detector_ConfigDlg::IDD, NULL)
	, mpOwnObjPtr(pOwnObjPtr)
{
	// iniPath 초기화
	GetCurrentDirectory(MAX_PATH, iniPath);
	wcscat_s(iniPath, L"\\config.ini"); 

	// mPath 초기화
	TCHAR imagePath[MAX_PATH];
	GetPrivateProfileString(INI_CATEGORY, INI_PARAMETER_PATH, L"C:\\", imagePath, MAX_PATH, iniPath);
	mPath.Append(imagePath);

	// mPort 초기화
	mPort = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_PORT, 8080, iniPath);

	// mDueDate, mComboSel 
	mDueDate = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_DUEDATE, -1, iniPath);
	mComboSel = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_COMBOSEL, 0, iniPath);

	// mToday 초기화
	SYSTEMTIME st;
	GetLocalTime(&st);
	mToday = st.wDay;
}

Motion_Detector_ConfigDlg::~Motion_Detector_ConfigDlg()
{
}

void Motion_Detector_ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAVE_DATE_COMBO, mComboBox);
	DDX_Text(pDX, IDC_PORT_EDIT, mPort);
}


BEGIN_MESSAGE_MAP(Motion_Detector_ConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &Motion_Detector_ConfigDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_OK, &Motion_Detector_ConfigDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &Motion_Detector_ConfigDlg::OnBnClickedBtnBrowse)
	ON_EN_CHANGE(IDC_PORT_EDIT, &Motion_Detector_ConfigDlg::OnEnChangePortEdit)
	ON_CBN_SELCHANGE(IDC_SAVE_DATE_COMBO, &Motion_Detector_ConfigDlg::OnCbnSelchangeSaveDateCombo)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// Motion_Detector_ConfigDlg 메시지 처리기입니다.
BOOL Motion_Detector_ConfigDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL Motion_Detector_ConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// combobox 값 매핑 배열 초기화
	mComboList2Date[0] = -1;
	mComboList2Date[1] = 1;
	mComboList2Date[2] = 3;
	mComboList2Date[3] = 7;
	mComboList2Date[4] = 14;
	mComboList2Date[5] = 21;
	mComboList2Date[6] = 30;
	mComboList2Date[7] = 60;
	mComboList2Date[8] = 90;
	mComboList2Date[9] = 120;
	mComboList2Date[10] = 150;
	mComboList2Date[11] = 180;
	mComboList2Date[12] = 365;
	mComboList2Date[13] = 730;

	// combobox 초기화
	CString combo_value[] = 
		{L"사용안함", L"1일", L"3일", L"1주", L"2주", L"3주", L"1개월", L"2개월", L"3개월", L"4개월", L"5개월", L"6개월",L"1년", L"2년"};
	 
	for(int i=0; i<14; i++)
		mComboBox.AddString(combo_value[i]);
	mComboBox.SetCurSel(mComboSel);

	return TRUE;
}

void Motion_Detector_ConfigDlg::OnBnClickedBtnOk()
{
	CString *message = new CString();
	message->Format(L"%s;%d;%d;%d", mPath,mPort, mDueDate,mComboSel);

	GetParent()->PostMessageW(UM_SAVE_POSTMESSAGE,0,(LPARAM)message);
	
	OnOK();
}

void Motion_Detector_ConfigDlg::OnBnClickedBtnCancel()
{
	OnCancel();
}


void Motion_Detector_ConfigDlg::OnBnClickedBtnBrowse()
{
	UpdateData(TRUE);

	// 디렉토리 선택 다이얼로그
	ITEMIDLIST*     pidlBrowse;
	TCHAR			pszPathname[1024];

	BROWSEINFO BrInfo;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.hwndOwner = this->GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = L"감시 영상을 저장할 폴더를 선택하세요";
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 다이얼로그를 띄우기
	pidlBrowse = SHBrowseForFolder(&BrInfo);

	if( pidlBrowse == NULL) return;

	// 선택한 폴더를 얻어옴
	::SHGetPathFromIDList(pidlBrowse, pszPathname);	
	mPath = pszPathname;

	// 폴더 이름 맨 뒤에 항상 \\ 붙여주기
	CString strRight = mPath.Right(1);
	if( strRight != L"\\" )
		mPath.Append(L"\\");

	UpdateData(FALSE);
}


void Motion_Detector_ConfigDlg::OnEnChangePortEdit()
{
	// PORT_EDIT 변경 시
	CString strPort;
	GetDlgItemText(IDC_PORT_EDIT, strPort);

	// mPort 업데이트
	mPort = _ttoi(strPort);
}

void Motion_Detector_ConfigDlg::OnCbnSelchangeSaveDateCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = mComboBox.GetCurSel();
    if(nIndex == -1)
        return;

	// ComboSel 구하기
	mComboSel = nIndex;
	
	// DueDate 구하기
	mDueDate = mComboList2Date[nIndex];
}

void Motion_Detector_ConfigDlg::OnClose()
{
	DestroyWindow();
}

void Motion_Detector_ConfigDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	mpOwnObjPtr = NULL;
	delete this;

	//이거 찾아봐야겟네 --
}
