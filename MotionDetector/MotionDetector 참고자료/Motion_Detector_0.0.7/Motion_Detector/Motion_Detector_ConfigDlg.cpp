// Motion_Detector_ConfigDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Motion_Detector_ConfigDlg.h"
#include "afxdialogex.h"


// Motion_Detector_ConfigDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Motion_Detector_ConfigDlg, CDialogEx)

Motion_Detector_ConfigDlg::Motion_Detector_ConfigDlg(Motion_Detector_ConfigDlg*& pOwnObjPtr)
	: CDialogEx(Motion_Detector_ConfigDlg::IDD, NULL)
	, mpOwnObjPtr(pOwnObjPtr)
{
	// iniPath �ʱ�ȭ
	GetCurrentDirectory(MAX_PATH, iniPath);
	wcscat_s(iniPath, L"\\config.ini"); 

	// mPath �ʱ�ȭ
	TCHAR imagePath[MAX_PATH];
	GetPrivateProfileString(INI_CATEGORY, INI_PARAMETER_PATH, L"C:\\", imagePath, MAX_PATH, iniPath);
	mPath.Append(imagePath);

	// mPort �ʱ�ȭ
	mPort = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_PORT, 8080, iniPath);

	// mDueDate, mComboSel 
	mDueDate = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_DUEDATE, -1, iniPath);
	mComboSel = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_COMBOSEL, 0, iniPath);

	// mToday �ʱ�ȭ
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


// Motion_Detector_ConfigDlg �޽��� ó�����Դϴ�.
BOOL Motion_Detector_ConfigDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL Motion_Detector_ConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// combobox �� ���� �迭 �ʱ�ȭ
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

	// combobox �ʱ�ȭ
	CString combo_value[] = 
		{L"������", L"1��", L"3��", L"1��", L"2��", L"3��", L"1����", L"2����", L"3����", L"4����", L"5����", L"6����",L"1��", L"2��"};
	 
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

	// ���丮 ���� ���̾�α�
	ITEMIDLIST*     pidlBrowse;
	TCHAR			pszPathname[1024];

	BROWSEINFO BrInfo;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.hwndOwner = this->GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = L"���� ������ ������ ������ �����ϼ���";
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// ���̾�α׸� ����
	pidlBrowse = SHBrowseForFolder(&BrInfo);

	if( pidlBrowse == NULL) return;

	// ������ ������ ����
	::SHGetPathFromIDList(pidlBrowse, pszPathname);	
	mPath = pszPathname;

	// ���� �̸� �� �ڿ� �׻� \\ �ٿ��ֱ�
	CString strRight = mPath.Right(1);
	if( strRight != L"\\" )
		mPath.Append(L"\\");

	UpdateData(FALSE);
}


void Motion_Detector_ConfigDlg::OnEnChangePortEdit()
{
	// PORT_EDIT ���� ��
	CString strPort;
	GetDlgItemText(IDC_PORT_EDIT, strPort);

	// mPort ������Ʈ
	mPort = _ttoi(strPort);
}

void Motion_Detector_ConfigDlg::OnCbnSelchangeSaveDateCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nIndex = mComboBox.GetCurSel();
    if(nIndex == -1)
        return;

	// ComboSel ���ϱ�
	mComboSel = nIndex;
	
	// DueDate ���ϱ�
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

	//�̰� ã�ƺ��߰ٳ� --
}
