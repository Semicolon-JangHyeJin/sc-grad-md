
// Motion_DetectorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Motion_Detector.h"
#include "Motion_DetectorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMotion_DetectorDlg ��ȭ ����
CMotion_DetectorDlg::CMotion_DetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotion_DetectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// iniPath �ʱ�ȭ
	GetCurrentDirectory(MAX_PATH, iniPath);
	wcscat_s(iniPath, L"\\config.ini"); 

	// mDiffLevel �ʱ�ȭ
	mDiffLevel = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_DIFF, 7, iniPath);

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

void CMotion_DetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM_DISPLAY, mDisplay);
	DDX_Control(pDX, IDC_DIFF_SLIDER, mSlider);
	DDX_Control(pDX, IDC_CAPTURE_LIST, mList);
	DDX_Text(pDX, IDC_DIFF_RATIO_EDIT, mDiffLevel);
}

BEGIN_MESSAGE_MAP(CMotion_DetectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_CAPTURE_LIST, &CMotion_DetectorDlg::OnNMDblclkCaptureList)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_DIFF_SLIDER, &CMotion_DetectorDlg::OnNMReleasedcaptureDiffSlider)
	ON_EN_CHANGE(IDC_DIFF_RATIO_EDIT, &CMotion_DetectorDlg::OnEnChangeDiffRatioEdit)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_START, &CMotion_DetectorDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMotion_DetectorDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_CONFIG, &CMotion_DetectorDlg::OnBnClickedBtnConfig)
	ON_MESSAGE(UM_SAVE_POSTMESSAGE, &CMotion_DetectorDlg::OnSaveMessage)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMotion_DetectorDlg �޽��� ó����

BOOL CMotion_DetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// ================================
	// TODO : ����� �߰� �κ�
	// ũ�� ���� �����ϵ��� ��Ÿ�� �߰�
	this->ModifyStyle(NULL, WS_THICKFRAME);

	// skip_frame_cnt �ʱ�ȭ
	skip_frame_cnt = 0;

	// slider control �ʱ�ȭ
	mSlider.SetRange(1, 9);
	mSlider.SetRangeMin(1);
	mSlider.SetRangeMax(9);
	mSlider.SetPos(mDiffLevel);
	mSlider.SetLineSize(1);
	mSlider.SetPageSize(1);

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

	// imagelist ���� �� list control �� ����
	mImageList.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR24, 0, 1);
	mList.SetImageList(&mImageList, LVSIL_NORMAL);

	// ������ ����Ʈ ǥ��
	ShowThumbnailList();

	// ImageStreaminServer �ʱ�ȭ
	pISS = new ImageStreamingServer();

	// ImagerStreamingServer ����
	if( pISS->Start(mPort) ) {
		// Ÿ�̸� ����
		SetTimer(_DEF_WEBCAM, TIMER_FRAME, NULL);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMotion_DetectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMotion_DetectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ������ ��� 
		if( pISS->isRunning )
		{
			// ���� �̹���
			CDC*	pDC;
			CRect	rect;

			// ������ ������ �κ��� rect�� ����
			pDC = mDisplay.GetDC();
			mDisplay.GetClientRect(&rect);

			// CvvImage�� ����� ������κп� �׸���.
			mCvvImage.CopyOf(pISS->currentImage);
			mCvvImage.DrawToHDC(pDC->m_hDC, rect);
			mCvvImage.Destroy();

			// �̹��� �Ҵ�����
			ReleaseDC(pDC);
		}

		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMotion_DetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMotion_DetectorDlg::OnBnClickedBtnStart()
{
	// ImagerStreamingServer ����
	if( pISS->Start(mPort) ) {
		// Ÿ�̸� ����
		SetTimer(_DEF_WEBCAM, TIMER_FRAME, NULL);
	}
}


void CMotion_DetectorDlg::OnBnClickedBtnStop()
{
	if( pISS->isRunning ) { 
		// ImageStreamingtServer ����
		pISS->Stop();

		// Ÿ�̸� ����
		KillTimer(_DEF_WEBCAM);
	}
}


void CMotion_DetectorDlg::OnDestroy()
{	
	// ��ü ����
	if(pISS != NULL) {
		delete pISS;
		pISS = NULL;
	}

	CDialogEx::OnDestroy();
}


void CMotion_DetectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == _DEF_WEBCAM )
	{
		// ���� �������� currentImage�� ���� & ���� object ���� �޾ƿ�
		cobject_cnt = pISS->GrabFrame(mDiffLevel);

		// LEARN_FRAME ��ŭ ����
		if(skip_frame_cnt < LEARN_FRAME) {
			skip_frame_cnt++;
		} else if(mDiffLevel != 1 && cobject_cnt != bobject_cnt) {
#if DEBUG_CAPTURE
			char text[50];
			CvFont font;

			sprintf (text,"Level=%d, Current=%d, Before=%d", mDiffLevel, cobject_cnt, bobject_cnt);
			cvInitFont (&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1,1);
			cvPutText (pISS->currentImage, text, cvPoint (4, pISS->currentImage->height - 10), &font, CV_RGB (255, 255, 255));
#endif
			SaveDetectImage();
		}

		// ���� object ������ ���� object ������ ��
		bobject_cnt = cobject_cnt;
		// currentImage -> beforeImage ����
		pISS->CopyCurrentToBefore();

		// display �κи� �ٽ� �׸���
		CRect rect;
		mList.GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect, FALSE);

		// ��¥ �˻�
		SYSTEMTIME st;
		GetLocalTime(&st);
		if(mToday != st.wDay && mDueDate != -1) {
			CString strDueDate;

			mDueDate--;

			if(mDueDate == 0) {
				removeAllImages();
					
				mDueDate = mComboList2Date[mComboSel];
			} 
			strDueDate.Format(L"%d", mDueDate);
			// ini ���� ������Ʈ
			WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DUEDATE, strDueDate, iniPath);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


// ������ ���� Ŭ�� �̺�Ʈ
void CMotion_DetectorDlg::OnNMDblclkCaptureList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(pNMItemActivate->iItem != -1) {
		// �׸����� ����
		CString strFullPath = mPath + mList.GetItemText(pNMItemActivate->iItem, 0);
		TCHAR strExecutable[FILENAME_MAX];
		int result = (int)FindExecutable(strFullPath, NULL, (LPTSTR)&strExecutable);
		
		if (result == 31) {         // ���� ����� ���α׷��� ���ٸ�
			SHELLEXECUTEINFO sei = 
					{sizeof(sei), 0, m_hWnd, L"Openas", strFullPath, NULL, NULL, 
					SW_SHOWNORMAL, AfxGetApp()->m_hInstance};  // ���̾�α� ����
			ShellExecuteEx(&sei);     
		} else if (result >= 32 ) {          // ����� ���α׷��� ã�ҽ��ϴ�.
			SHELLEXECUTEINFO sei = 
					{sizeof(sei), 0, m_hWnd, L"Open", strFullPath, NULL, NULL, 
					SW_SHOWNORMAL, AfxGetApp()->m_hInstance}; // ���� ����
		   ShellExecuteEx(&sei);
		} else {         // ���� �߻�
			AfxMessageBox(L"����ġ ���� ������ �߻��Ͽ����ϴ�.");
		}
	}

	*pResult = 0;
}

void CMotion_DetectorDlg::ShowThumbnailList() {
	// nIndex �� image list �ʱ�ȭ
	nIndex = 0;
	for(int i=0;i<mImageList.GetImageCount();i++)
		mImageList.Remove(i);
	mImageList.SetImageCount(nIndex);

	mList.DeleteAllItems();

	// ĸ�� ���� ã��
	CFileFind finder;
	CString strWildCard(mPath);
	strWildCard += "\\cam-*.jpg";
	IplImage* image = NULL; 
	vector<CString> strFileList;

	BOOL bWorking = finder.FindFile( strWildCard );
	while(bWorking) {
		bWorking = finder.FindNextFile();
		
		if( finder.IsDots() || finder.IsDirectory() )
			continue;  
		else {
			// ã������ strFileList vector�� �ֱ�
			strFileList.push_back(finder.GetFileName());
		}
	} 
	finder.Close();

	// ���������� ���� ����Ʈ�� ������������ ����
	reverse(strFileList.begin(), strFileList.end());
	for(unsigned int i=0;i<strFileList.size();i++) {
		// 10�������� �����ֱ�
		if(i >= 10)
			break;

		image = cvLoadImage(CT2A(mPath+"\\"+strFileList[i]), CV_LOAD_IMAGE_UNCHANGED); // load image
		AddThumbnailList(strFileList[i], image);
		if(image != NULL) cvReleaseImage(&image); // thumbnail release
	}
}

void CMotion_DetectorDlg::SaveDetectImage() {
	// ������ �̸��� ���� �ð��� �����Ͽ� �����Ѵ�.
	SYSTEMTIME st;
	GetLocalTime(&st);

	
	/*CString strVolDrive, strFree;
	AfxExtractSubString(strVolDrive, mPath, 0, '\\');
	int free = GetFreeSpaceOfDrive(strVolDrive+'\\');
	strFree.Format(L"%d", free);
	AfxMessageBox(strFree);*/

	CString strFileName, strFullPath;
	strFileName.Format(L"cam-%02d%02d%02d-%02d%02d%02d.jpg", 
		st.wYear-2000, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond);
	strFullPath = mPath + strFileName;

	// OpenCV �Լ��� �̿��Ͽ� jpeg ���� �������� �����Ѵ�.
	IplImage* saveImage = cvCreateImage(cvSize(SAVED_WIDTH,SAVED_HEIGHT),IPL_DEPTH_8U,3);
	cvResize(pISS->currentImage, saveImage, CV_INTER_LINEAR);
	cvSaveImage(CT2A(strFullPath), saveImage);
	if(saveImage != NULL) cvReleaseImage(&saveImage);

	// ������ ����Ʈ ǥ��
	ShowThumbnailList();
}

void CMotion_DetectorDlg::AddThumbnailList(CString FileName, IplImage* image) {
	// thumbnail �����
	IplImage* thumb = cvCreateImage(cvSize(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT),IPL_DEPTH_8U,3);
	cvResize(image, thumb, CV_INTER_LINEAR);

	// ����Ʈ�� �߰�
	InsertList(thumb, FileName);

	// thumbnail release
	if(thumb != NULL) cvReleaseImage(&thumb);
}

void CMotion_DetectorDlg::InsertList(IplImage* thumb, CString FileName) {
	UpdateData(TRUE);
	LVITEM Item;

	// image list ���� ����
	mImageList.SetImageCount(nIndex+1);

	// ����Ʈ�� ������ �߰�
	Item.mask = LVIF_IMAGE | LVIF_TEXT;  
	Item.iItem = nIndex;  
	Item.iSubItem = 0;
	Item.pszText = FileName.GetBuffer(FileName.GetLength());
	Item.iImage = nIndex;
	mList.InsertItem(&Item);

	// thumbnail �׸���
	CBitmap ThumbBitmap;
	HBITMAP bm = iplimage2hbitmap(thumb);
	ThumbBitmap.Attach(bm);
	mImageList.Replace(nIndex, &ThumbBitmap, NULL);
	mList.RedrawItems(nIndex, nIndex);

	// bm release
	DeleteObject(bm);

	nIndex++;
}

void CMotion_DetectorDlg::removeAllImages() {
	// ĸ�� ���� ã��
	CFileFind finder;
	CString strWildCard(mPath);
	strWildCard += "\\cam-*.jpg";

	BOOL bWorking = finder.FindFile( strWildCard );
	while(bWorking) {
		bWorking = finder.FindNextFile();
		
		if( finder.IsDots() || finder.IsDirectory() )
			continue;  
		else {
			// ã������ ����
			DeleteFile(mPath + finder.GetFileName());
		}
	} 
	finder.Close();

	ShowThumbnailList();
}

int CMotion_DetectorDlg::GetFreeSpaceOfDrive(CString strVolDrive) {
  ULARGE_INTEGER avail, total, free;
  GetDiskFreeSpaceEx(strVolDrive, &avail, &total, &free );
  
  //MBytes�� ó�� 
  int disk_free_size = (int)(free.QuadPart>>20);

  return disk_free_size ;
}

void CMotion_DetectorDlg::OnNMReleasedcaptureDiffSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// thumb�� ���� ���� �� ��� 
	// �����̴� thumb �� ��ġ�� �޾Ƽ� ������Ʈ
	mDiffLevel = mSlider.GetPos();
	UpdateData(FALSE);

	// ini ���� ������Ʈ
	CString strRatio;
	strRatio.Format(L"%d", mDiffLevel);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DIFF, strRatio, iniPath);

	*pResult = 0;
}

void CMotion_DetectorDlg::OnEnChangeDiffRatioEdit()
{
	// Diff_RATIO_EDIT �� ���� ���� ��
	UpdateData(TRUE);
	CString strRatio;
	GetDlgItemText(IDC_DIFF_RATIO_EDIT, strRatio);

	// slider control ������Ʈ
	mDiffLevel = _ttoi(strRatio);
	mSlider.SetPos(mDiffLevel);

	// ini ���� ������Ʈ
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DIFF, strRatio, iniPath);
}

void CMotion_DetectorDlg::OnBnClickedBtnConfig()
{
	// config dialog ����
	mMotion_Detector_ConfigDlg = new Motion_Detector_ConfigDlg(mMotion_Detector_ConfigDlg);
	mMotion_Detector_ConfigDlg->Create(this);
	mMotion_Detector_ConfigDlg->ShowWindow(SW_SHOWNORMAL);
}

BOOL CMotion_DetectorDlg::PreTranslateMessage(MSG* pMsg)
{
	// esc ������ ������ �ʰ� �ϱ�
	if(NULL != pMsg)
	{
		if(WM_KEYDOWN == pMsg->message)
		{
			if( VK_RETURN == pMsg->wParam ||
			    VK_ESCAPE == pMsg->wParam )
			{
				return FALSE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMotion_DetectorDlg::OnSaveMessage(WPARAM wParam, LPARAM lParam)
{
	CString *recv = (CString *)lParam;
	CString strPath, strPort, strDueDate, strComboSel;

	// config dialog���� ���� message�� ����
	AfxExtractSubString(strPath, *recv, 0,';');
	AfxExtractSubString(strPort, *recv, 1,';');
	AfxExtractSubString(strDueDate, *recv, 2,';');
	AfxExtractSubString(strComboSel, *recv, 3,';');

	mPath = strPath;
	mPort = _ttoi(strPort);
	mDueDate = _ttoi(strDueDate);
	mComboSel = _ttoi(strComboSel);

	//AfxMessageBox(*recv);
	if( recv != NULL ) {
		delete recv;
		recv = NULL;
	}

	// ����Ʈ ���� ��ħ
	ShowThumbnailList();

	// ini ���� ����
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_PATH, mPath, iniPath);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_PORT, strPort, iniPath);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DUEDATE, strDueDate, iniPath);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_COMBOSEL, strComboSel, iniPath);

	return TRUE;
}

void CMotion_DetectorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// �� control �� width ���
	CRect rect, rect_tmp;
	int staticGroupHeight,buttonHeight,listWidth, listHeight;
	int displayWidth, displayHeight;
	CStatic *static_group;
	CButton *button;

	static_group = (CStatic *)GetDlgItem(IDC_STATIC_GROUP);
	if(static_group != NULL) {
		static_group->GetWindowRect(&rect);
		ScreenToClient(&rect);
		staticGroupHeight = rect.Height();
	}

	button = (CButton *)GetDlgItem(IDC_BTN_START);
	if(button != NULL) {
		button->GetWindowRect(&rect);
		ScreenToClient(&rect);
		buttonHeight = rect.Height();
	}

	mList.GetWindowRect(&rect);
	ScreenToClient(&rect);
	listWidth = rect.Width();
	listHeight = cy - 40 - staticGroupHeight - buttonHeight;

	// display �κ� ũ�� ����
	displayWidth = cx - 30 - listWidth;
	displayHeight = cy - 20;
	mDisplay.MoveWindow(10,10, displayWidth, displayHeight);
	
	// static text �� group, EditText �̵�
	CStatic *statics = (CStatic *)GetDlgItem(IDC_STATIC_LEVEL);
	CEdit *edit = (CEdit *)GetDlgItem(IDC_DIFF_RATIO_EDIT);
	
	if(statics != NULL && edit != NULL) {
		statics->GetWindowRect(&rect);
		edit->GetWindowRect(&rect_tmp);
		ScreenToClient(&rect);
		ScreenToClient(&rect_tmp);
		rect_tmp.MoveToX(displayWidth + 30 + rect_tmp.left - rect.left);
		rect.MoveToX(displayWidth + 30);
		statics->MoveWindow(&rect);
		edit->MoveWindow(&rect_tmp);
	}

	statics = (CStatic *)GetDlgItem(IDC_STATIC_LOW);
	CStatic *static_tmp = (CStatic *)GetDlgItem(IDC_STATIC_HIGH);

	if(statics != NULL && static_tmp != NULL) {
		statics->GetWindowRect(&rect);
		static_tmp->GetWindowRect(&rect_tmp);
		ScreenToClient(&rect);
		ScreenToClient(&rect_tmp);
		rect_tmp.MoveToX(displayWidth + 30 + rect_tmp.left - rect.left);
		rect.MoveToX(displayWidth + 30);
		statics->MoveWindow(&rect);
		static_tmp->MoveWindow(&rect_tmp);
	}

	statics = (CStatic *)GetDlgItem(IDC_STATIC_GROUP);

	if(statics != NULL) {
		statics->GetWindowRect(&rect_tmp);
		ScreenToClient(&rect_tmp);
		rect_tmp.MoveToX(displayWidth + 20);
		statics->MoveWindow(&rect_tmp);
	}

	// Slider �̵�
	mSlider.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.MoveToX(displayWidth + 30);
	mSlider.MoveWindow(&rect);

	// ��޼��� ��ư �̵�
	CButton *btns = (CButton *)GetDlgItem(IDC_BTN_CONFIG);

	if(btns != NULL) {
		btns->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.MoveToX(displayWidth + 30);
		btns->MoveWindow(&rect);
	}

	// List ũ�� ���� �� �̵�
	mList.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.MoveToX(displayWidth + 20);
	mList.MoveWindow(rect.left, rect.top, rect.Width(), listHeight);

	//����, ���� ��ư �̵�
	btns = (CButton *)GetDlgItem(IDC_BTN_START);
	CButton *btn_tmp = (CButton *)GetDlgItem(IDC_BTN_STOP);
	if(btns != NULL && btn_tmp != NULL) {
		btns->GetWindowRect(&rect);
		btn_tmp->GetWindowRect(&rect_tmp);
		ScreenToClient(&rect);
		ScreenToClient(&rect_tmp);
		rect_tmp.MoveToXY(displayWidth + 20 + rect_tmp.left - rect.left, cy - 10 - buttonHeight);
		rect.MoveToXY(displayWidth + 20, cy - 10 - buttonHeight);
		btns->MoveWindow(&rect);
		btn_tmp->MoveWindow(&rect_tmp);
	}

	// ������ ��� invalidate
	Invalidate(TRUE);
}