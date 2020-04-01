
// Motion_DetectorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Motion_Detector.h"
#include "Motion_DetectorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CMotion_DetectorDlg 대화 상자
CMotion_DetectorDlg::CMotion_DetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotion_DetectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// iniPath 초기화
	GetCurrentDirectory(MAX_PATH, iniPath);
	wcscat_s(iniPath, L"\\config.ini"); 

	// mDiffLevel 초기화
	mDiffLevel = GetPrivateProfileInt(INI_CATEGORY, INI_PARAMETER_DIFF, 7, iniPath);

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


// CMotion_DetectorDlg 메시지 처리기

BOOL CMotion_DetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// ================================
	// TODO : 사용자 추가 부분
	// 크기 조절 가능하도록 스타일 추가
	this->ModifyStyle(NULL, WS_THICKFRAME);

	// skip_frame_cnt 초기화
	skip_frame_cnt = 0;

	// slider control 초기화
	mSlider.SetRange(1, 9);
	mSlider.SetRangeMin(1);
	mSlider.SetRangeMax(9);
	mSlider.SetPos(mDiffLevel);
	mSlider.SetLineSize(1);
	mSlider.SetPageSize(1);

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

	// imagelist 생성 및 list control 과 연결
	mImageList.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR24, 0, 1);
	mList.SetImageList(&mImageList, LVSIL_NORMAL);

	// 섬네일 리스트 표시
	ShowThumbnailList();

	// ImageStreaminServer 초기화
	pISS = new ImageStreamingServer();

	// ImagerStreamingServer 시작
	if( pISS->Start(mPort) ) {
		// 타이머 시작
		SetTimer(_DEF_WEBCAM, TIMER_FRAME, NULL);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMotion_DetectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 동영상 출력 
		if( pISS->isRunning )
		{
			// 메인 이미지
			CDC*	pDC;
			CRect	rect;

			// 동영상 나오는 부분을 rect에 저장
			pDC = mDisplay.GetDC();
			mDisplay.GetClientRect(&rect);

			// CvvImage를 만들어 동영상부분에 그린다.
			mCvvImage.CopyOf(pISS->currentImage);
			mCvvImage.DrawToHDC(pDC->m_hDC, rect);
			mCvvImage.Destroy();

			// 이미지 할당해제
			ReleaseDC(pDC);
		}

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMotion_DetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMotion_DetectorDlg::OnBnClickedBtnStart()
{
	// ImagerStreamingServer 시작
	if( pISS->Start(mPort) ) {
		// 타이머 시작
		SetTimer(_DEF_WEBCAM, TIMER_FRAME, NULL);
	}
}


void CMotion_DetectorDlg::OnBnClickedBtnStop()
{
	if( pISS->isRunning ) { 
		// ImageStreamingtServer 종료
		pISS->Stop();

		// 타이머 종료
		KillTimer(_DEF_WEBCAM);
	}
}


void CMotion_DetectorDlg::OnDestroy()
{	
	// 객체 제거
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
		// 현재 프레임을 currentImage에 저장 & 현재 object 갯수 받아옴
		cobject_cnt = pISS->GrabFrame(mDiffLevel);

		// LEARN_FRAME 만큼 무시
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

		// 현재 object 갯수가 이전 object 갯수가 됨
		bobject_cnt = cobject_cnt;
		// currentImage -> beforeImage 복사
		pISS->CopyCurrentToBefore();

		// display 부분만 다시 그리기
		CRect rect;
		mList.GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect, FALSE);

		// 날짜 검사
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
			// ini 파일 업데이트
			WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DUEDATE, strDueDate, iniPath);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


// 아이콘 더블 클릭 이벤트
void CMotion_DetectorDlg::OnNMDblclkCaptureList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMItemActivate->iItem != -1) {
		// 그림파일 열기
		CString strFullPath = mPath + mList.GetItemText(pNMItemActivate->iItem, 0);
		TCHAR strExecutable[FILENAME_MAX];
		int result = (int)FindExecutable(strFullPath, NULL, (LPTSTR)&strExecutable);
		
		if (result == 31) {         // 만약 연결된 프로그램이 없다면
			SHELLEXECUTEINFO sei = 
					{sizeof(sei), 0, m_hWnd, L"Openas", strFullPath, NULL, NULL, 
					SW_SHOWNORMAL, AfxGetApp()->m_hInstance};  // 다이얼로그 띄우기
			ShellExecuteEx(&sei);     
		} else if (result >= 32 ) {          // 연결된 프로그램을 찾았습니다.
			SHELLEXECUTEINFO sei = 
					{sizeof(sei), 0, m_hWnd, L"Open", strFullPath, NULL, NULL, 
					SW_SHOWNORMAL, AfxGetApp()->m_hInstance}; // 파일 실행
		   ShellExecuteEx(&sei);
		} else {         // 에러 발생
			AfxMessageBox(L"예기치 못한 에러가 발생하였습니다.");
		}
	}

	*pResult = 0;
}

void CMotion_DetectorDlg::ShowThumbnailList() {
	// nIndex 및 image list 초기화
	nIndex = 0;
	for(int i=0;i<mImageList.GetImageCount();i++)
		mImageList.Remove(i);
	mImageList.SetImageCount(nIndex);

	mList.DeleteAllItems();

	// 캡쳐 파일 찾기
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
			// 찾았으면 strFileList vector에 넣기
			strFileList.push_back(finder.GetFileName());
		}
	} 
	finder.Close();

	// 오름차순인 파일 리스트를 내림차순으로 변경
	reverse(strFileList.begin(), strFileList.end());
	for(unsigned int i=0;i<strFileList.size();i++) {
		// 10개까지만 보여주기
		if(i >= 10)
			break;

		image = cvLoadImage(CT2A(mPath+"\\"+strFileList[i]), CV_LOAD_IMAGE_UNCHANGED); // load image
		AddThumbnailList(strFileList[i], image);
		if(image != NULL) cvReleaseImage(&image); // thumbnail release
	}
}

void CMotion_DetectorDlg::SaveDetectImage() {
	// 파일의 이름은 현재 시간을 참고하여 생성한다.
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

	// OpenCV 함수를 이용하여 jpeg 파일 포맷으로 저장한다.
	IplImage* saveImage = cvCreateImage(cvSize(SAVED_WIDTH,SAVED_HEIGHT),IPL_DEPTH_8U,3);
	cvResize(pISS->currentImage, saveImage, CV_INTER_LINEAR);
	cvSaveImage(CT2A(strFullPath), saveImage);
	if(saveImage != NULL) cvReleaseImage(&saveImage);

	// 섬네일 리스트 표시
	ShowThumbnailList();
}

void CMotion_DetectorDlg::AddThumbnailList(CString FileName, IplImage* image) {
	// thumbnail 만들기
	IplImage* thumb = cvCreateImage(cvSize(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT),IPL_DEPTH_8U,3);
	cvResize(image, thumb, CV_INTER_LINEAR);

	// 리스트에 추가
	InsertList(thumb, FileName);

	// thumbnail release
	if(thumb != NULL) cvReleaseImage(&thumb);
}

void CMotion_DetectorDlg::InsertList(IplImage* thumb, CString FileName) {
	UpdateData(TRUE);
	LVITEM Item;

	// image list 길이 정의
	mImageList.SetImageCount(nIndex+1);

	// 리스트에 아이템 추가
	Item.mask = LVIF_IMAGE | LVIF_TEXT;  
	Item.iItem = nIndex;  
	Item.iSubItem = 0;
	Item.pszText = FileName.GetBuffer(FileName.GetLength());
	Item.iImage = nIndex;
	mList.InsertItem(&Item);

	// thumbnail 그리기
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
	// 캡쳐 파일 찾기
	CFileFind finder;
	CString strWildCard(mPath);
	strWildCard += "\\cam-*.jpg";

	BOOL bWorking = finder.FindFile( strWildCard );
	while(bWorking) {
		bWorking = finder.FindNextFile();
		
		if( finder.IsDots() || finder.IsDirectory() )
			continue;  
		else {
			// 찾았으면 삭제
			DeleteFile(mPath + finder.GetFileName());
		}
	} 
	finder.Close();

	ShowThumbnailList();
}

int CMotion_DetectorDlg::GetFreeSpaceOfDrive(CString strVolDrive) {
  ULARGE_INTEGER avail, total, free;
  GetDiskFreeSpaceEx(strVolDrive, &avail, &total, &free );
  
  //MBytes로 처리 
  int disk_free_size = (int)(free.QuadPart>>20);

  return disk_free_size ;
}

void CMotion_DetectorDlg::OnNMReleasedcaptureDiffSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// thumb가 선택 해제 된 경우 
	// 슬라이더 thumb 의 위치를 받아서 업데이트
	mDiffLevel = mSlider.GetPos();
	UpdateData(FALSE);

	// ini 파일 업데이트
	CString strRatio;
	strRatio.Format(L"%d", mDiffLevel);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DIFF, strRatio, iniPath);

	*pResult = 0;
}

void CMotion_DetectorDlg::OnEnChangeDiffRatioEdit()
{
	// Diff_RATIO_EDIT 을 직접 변경 시
	UpdateData(TRUE);
	CString strRatio;
	GetDlgItemText(IDC_DIFF_RATIO_EDIT, strRatio);

	// slider control 업데이트
	mDiffLevel = _ttoi(strRatio);
	mSlider.SetPos(mDiffLevel);

	// ini 파일 업데이트
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DIFF, strRatio, iniPath);
}

void CMotion_DetectorDlg::OnBnClickedBtnConfig()
{
	// config dialog 띄우기
	mMotion_Detector_ConfigDlg = new Motion_Detector_ConfigDlg(mMotion_Detector_ConfigDlg);
	mMotion_Detector_ConfigDlg->Create(this);
	mMotion_Detector_ConfigDlg->ShowWindow(SW_SHOWNORMAL);
}

BOOL CMotion_DetectorDlg::PreTranslateMessage(MSG* pMsg)
{
	// esc 눌러도 꺼지지 않게 하기
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

	// config dialog에서 보낸 message를 나눔
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

	// 리스트 새로 고침
	ShowThumbnailList();

	// ini 파일 변경
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_PATH, mPath, iniPath);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_PORT, strPort, iniPath);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_DUEDATE, strDueDate, iniPath);
	WritePrivateProfileString(INI_CATEGORY, INI_PARAMETER_COMBOSEL, strComboSel, iniPath);

	return TRUE;
}

void CMotion_DetectorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// 각 control 의 width 계산
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

	// display 부분 크기 변경
	displayWidth = cx - 30 - listWidth;
	displayHeight = cy - 20;
	mDisplay.MoveWindow(10,10, displayWidth, displayHeight);
	
	// static text 및 group, EditText 이동
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

	// Slider 이동
	mSlider.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.MoveToX(displayWidth + 30);
	mSlider.MoveWindow(&rect);

	// 고급설정 버튼 이동
	CButton *btns = (CButton *)GetDlgItem(IDC_BTN_CONFIG);

	if(btns != NULL) {
		btns->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.MoveToX(displayWidth + 30);
		btns->MoveWindow(&rect);
	}

	// List 크기 조절 및 이동
	mList.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.MoveToX(displayWidth + 20);
	mList.MoveWindow(rect.left, rect.top, rect.Width(), listHeight);

	//시작, 중지 버튼 이동
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

	// 배경까지 모두 invalidate
	Invalidate(TRUE);
}