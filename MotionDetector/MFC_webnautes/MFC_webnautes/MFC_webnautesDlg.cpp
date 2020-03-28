
// MFC_webnautesDlg.cpp: 구현 파일
//
#pragma warning(disable:4996)

#include "pch.h"
#include "framework.h"
#include "MFC_webnautes.h"
#include "MFC_webnautesDlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include "MD_CONFIG.h"
#include "MD_Calendar.h"
#include <atlimage.h>

using namespace cv;
using namespace std;

VideoCapture* capture;
Mat mat_frame;
CImage cimage_mfc;
CString CapPath = _T("C:\\MD_Capture");
CImage m_image;

Mat frame, gray, frameDelta, thresh, firstFrame;
vector<vector<Point> > cnts;

int insert = 0;
//몇으로 해야해..? 찾아봐야지...
int sensor[11] = { 3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600, 1400, 1200 };
int setContour = 5;
CString selFolderDate = _T("");

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL myUtil_CheckDir(CString sDirName)
{
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(sDirName + strFile);

	if (!bResult)
	{
		bResult = CreateDirectory(sDirName + "\\", NULL);
		if (!bResult)
		{
			/*Error*/
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

CString currentDateTime() {
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
	struct tm  tstruct;
	char       buf[80];
	CString cbuf = _T("");
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링
	cbuf = LPSTR(buf);
	return cbuf;
}

CString currentDate() {
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
	struct tm  tstruct;
	char       buf[80];
	CString cbuf = _T("");
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링
	cbuf = LPSTR(buf);
	return cbuf;
}

//BOOL AddImage(LPCTSTR filename, CImageList& imagelist)
//{
//	CBitmap Bmp;
//	Bmp.Attach((HBITMAP)::LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
//	if ((HBITMAP)Bmp == NULL)
//		return FALSE;
//	imagelist.Add(&Bmp, RGB(255, 255, 255));
//}

void CMFCwebnautesDlg::capPicture(Mat image) {
	if (insert == 0) {
		//char savefile[200];
		CString tempDate = currentDate();
		CString tempfile = CapPath + "\\" + tempDate + "\\image_" + currentDateTime() + ".jpg";
		string savefile = CT2CA(tempfile);
		CString tempDir = CapPath + "\\" + tempDate;
		myUtil_CheckDir(tempDir);
		//string savefile = CapPath + "\\image_" + currentDateTime() + ".jpg";
		//sprintf(savefile, "image_%d.jpg", currentDateTime());
		//printf("cap %s\n", savefile);
		imwrite(savefile, image);        // img를 파일로 저장한다.
		insert++;
		ShowThumbnailList();
		//putText(image, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
	}
	else if (insert > 50) {
		//char savefile[200];
		CString tempDate = currentDate();
		CString tempfile = CapPath + "\\" + tempDate + "\\image_" + currentDateTime() + ".jpg";
		string savefile = CT2CA(tempfile);
		CString tempDir = CapPath + "\\" + tempDate;
		myUtil_CheckDir(tempDir);
		//string savefile = "image_" + currentDateTime() + ".jpg";
		//sprintf(savefile, "image_%d.jpg", currentDateTime());
		imwrite(savefile, image);        // img를 파일로 저장한다.
		//printf("cap %s\n", savefile);
		insert = 0;
		ShowThumbnailList();
		//putText(image, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
	}
	else {
		insert++;
	}

}

void CMFCwebnautesDlg::Running() {
	//Motion Detection 부분


	int v = 0;

	//sleep(3);

	capture->read(frame);
	//convert to grayscale and set the first frame
	cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
	GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

	while (capture->read(frame)) {

		//convert to grayscale
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(21, 21), 0);

		//compute difference between first frame and current frame
		absdiff(firstFrame, gray, frameDelta);
		threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);

		dilate(thresh, thresh, Mat(), Point(-1, -1), 2);
		findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < cnts.size(); i++) {
			if (contourArea(cnts[i]) < sensor[setContour]) {
				continue;
			}
			capPicture(frame);
		}

		//imshow("capture", frame);
		//camera.read(frame);
		Sleep(50);
		//convert to grayscale and set the first frame
		//v++;
		//if (v > 10) {
		cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
		GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
		//v = 0;
	//}
		if (waitKey(1) == 27) {
			//exit if ESC is pressed
			//break;
		}

	}
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCwebnautesDlg 대화 상자

CMFCwebnautesDlg::CMFCwebnautesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_WEBNAUTES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCwebnautesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_SLIDER1, m_sld);
	DDX_Control(pDX, IDC_EDIT1, m_ed);
	DDX_Control(pDX, IDCANCEL, m_list);
	DDX_Control(pDX, IDC_LIST1, m_piclist);
}

BEGIN_MESSAGE_MAP(CMFCwebnautesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_PICTURE, &CMFCwebnautesDlg::OnStnClickedPicture)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMFCwebnautesDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDOK, &CMFCwebnautesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCwebnautesDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCwebnautesDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCwebnautesDlg::OnEnChangeEdit1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCwebnautesDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMFCwebnautesDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// CMFCwebnautesDlg 메시지 처리기

BOOL CMFCwebnautesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//카메라 연결
	capture = new VideoCapture(1);
	if (!capture->isOpened())
	{
		capture = new VideoCapture(0);
		if (!capture->isOpened()) {
			MessageBox(_T("캠을 열수 없습니다. \n"));
		}
	}

	//감지 등급 Slide Control 설정
	m_sld.SetRange(0, 10); // 최소 최대 값
	m_sld.SetPos(5); // 현재 위치
	m_sld.SetLineSize(1); // 방향키로 움직일 때 사이즈
	m_sld.SetPageSize(2); //PgUp, PgDn으로 움직일 때 사이즈

	//감지 등급 Edit Box 설정
	int iPos = m_sld.GetPos();
	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_ed.SetWindowText(sPos);

	//웹캠 크기를  320x240으로 지정    
	/*capture->set(CAP_PROP_FRAME_WIDTH, 320);
	capture->set(CAP_PROP_FRAME_HEIGHT, 240);*/

	//이미지 로드
	//m_image.Load(L"C:\\MD_Capture\\image_20191116083355.jpg");
	myUtil_CheckDir(CapPath);

	//이미지 리스트 초기화
	//ShowImage();
	// imagelist 생성 및 list control 과 연결
	mImageList.Create(64, 64, ILC_COLOR8 | ILC_MASK, 8, 8);
	m_piclist.SetImageList(&mImageList, LVSIL_NORMAL);

	ShowThumbnailList();
	SetTimer(1000, 1000 / 15, NULL);
	
	//camera = new VideoCapture(1);
	//if (!camera->isOpened())
	//{
	//	MessageBox(_T("캠을 열수 없습니다. \n"));
	//}

	////웹캠 크기를  320x240으로 지정    
	//camera->set(CAP_PROP_FRAME_WIDTH, 320);
	//camera->set(CAP_PROP_FRAME_HEIGHT, 240);

	//SetTimer(1000, 30, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCwebnautesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCwebnautesDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCwebnautesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCwebnautesDlg::OnStnClickedPicture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CMFCwebnautesDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMFCwebnautesDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//sleep(3);

		  //mat_frame가 입력 이미지입니다. 
	capture->read(mat_frame);


	//이곳에 OpenCV 함수들을 적용합니다.

	//여기에서는 그레이스케일 이미지로 변환합니다.
	//cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);



	//화면에 보여주기 위한 처리입니다.
	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}



	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}


	RECT r;
	m_picture.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}


	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}


	HDC dc = ::GetDC(m_picture.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);

	::ReleaseDC(m_picture.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

	CDialogEx::OnTimer(nIDEvent);
}



void CMFCwebnautesDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	//감지 등급 Edit Box 설정
	int iPos = m_sld.GetPos();
	setContour = iPos;
	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_ed.SetWindowText(sPos);
}


void CMFCwebnautesDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
	capture = new VideoCapture(1);
	if (!capture->isOpened())
	{
		capture = new VideoCapture(0);
		if (!capture->isOpened()) {
			MessageBox(_T("캠을 열수 없습니다. \n"));
		}
	}
	Running();
}


void CMFCwebnautesDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//MD_CONFIG dlg;
	//dlg.DoModal(); 
	dlg = new MD_CONFIG(dlg);
	dlg->Create(IDD_DIALOG1);
	dlg->CenterWindow();
	dlg->ShowWindow(SW_SHOWNORMAL);
}


void CMFCwebnautesDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//달력 이용할 시,
	//cdlg = new MD_Calendar(cdlg);
	//cdlg->Create(IDD_DIALOG2);
	//cdlg->CenterWindow();
	//cdlg->ShowWindow(SW_SHOWNORMAL);
	ShellExecute(NULL, _T("open"), CapPath, NULL, NULL, SW_SHOW);
}


void CMFCwebnautesDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCwebnautesDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;

}

////////////////////////////////////////////////////////////////////////////////
void CMFCwebnautesDlg::ShowThumbnailList() {
	// nIndex 및 image list 초기화
	nIndex = 0;
	for (int i = 0; i < mImageList.GetImageCount(); i++)
		mImageList.Remove(i);
	mImageList.SetImageCount(nIndex);

	m_piclist.DeleteAllItems();

	// 캡쳐 파일 찾기
	CFileFind finder;
	CString strWildCard(CapPath);
	strWildCard += "\\";
	strWildCard += currentDate();
	strWildCard += "\\image_*.jpg";
	Mat image;
	vector<CString> strFileList;

	BOOL bWorking = finder.FindFile(strWildCard);
	while (bWorking) {
		bWorking = finder.FindNextFile();

		if (finder.IsDots() || finder.IsDirectory())
			continue;
		else {
			// 찾았으면 strFileList vector에 넣기
			strFileList.push_back(finder.GetFileName());
		}
	}
	finder.Close();
	
	CString tempStr(CapPath);
	tempStr += "\\";
	tempStr += currentDate();
	// 오름차순인 파일 리스트를 내림차순으로 변경
	reverse(strFileList.begin(), strFileList.end());
	for (unsigned int i = 0; i < strFileList.size(); i++) {
		// 10개까지만 보여주기
		if (i >= 10)
			break;
		CString temp = tempStr + "\\" + strFileList[i];
		CT2CA pszString(temp);
		std::string strPath(pszString);
		image = imread(strPath, IMREAD_UNCHANGED);
		//image = imread((char*)(LPCTSTR)(temp), IMREAD_UNCHANGED);
		//image = cvLoadImage(CT2A(CapPath + "\\" + strFileList[i]), CV_LOAD_IMAGE_UNCHANGED); // load image
		AddThumbnailList(strFileList[i], image);
		if (!(image.empty())) image.release();
		//if (image != NULL) cvReleaseImage(&image); // thumbnail release
	}
}

//void CMFCwebnautesDlg::SaveDetectImage() {
//	// 파일의 이름은 현재 시간을 참고하여 생성한다.
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//
//
//	/*CString strVolDrive, strFree;
//	AfxExtractSubString(strVolDrive, mPath, 0, '\\');
//	int free = GetFreeSpaceOfDrive(strVolDrive+'\\');
//	strFree.Format(L"%d", free);
//	AfxMessageBox(strFree);*/
//
//	CString strFileName, strFullPath;
//	strFileName.Format(L"cam-%02d%02d%02d-%02d%02d%02d.jpg",
//		st.wYear - 2000, st.wMonth, st.wDay,
//		st.wHour, st.wMinute, st.wSecond);
//	strFullPath = CapPath + strFileName;
//
//	// OpenCV 함수를 이용하여 jpeg 파일 포맷으로 저장한다.
//	//IplImage* saveImage = cvCreateImage(cvSize(SAVED_WIDTH, SAVED_HEIGHT), IPL_DEPTH_8U, 3);
//	Mat saveImage = Mat(Size(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT), CV_8UC3);
//	resize(currentImage, saveImage,Size(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT), 0, 0, INTER_LINEAR);
//	cvSaveImage(CT2A(strFullPath), saveImage);
//	if (saveImage != NULL) cvReleaseImage(&saveImage);
//
//	// 섬네일 리스트 표시
//	ShowThumbnailList();
//}

//HBITMAP CMFCwebnautesDlg::Mat2DIB(Mat* Image)
//{
//	HBITMAP hbmp = NULL;
//	int bpp = Image->channels() * 8;
//
//	Mat tmp;
//	cvtColor(*Image, tmp, CV_GRAY2BGR);
//
//	BITMAPINFO bmpInfo = { 0 };
//	LONG lBmpSize = Image->rows * Image->cols * 3;
//	bmpInfo.bmiHeader.biBitCount = bpp;
//	bmpInfo.bmiHeader.biHeight = Image->rows;
//	bmpInfo.bmiHeader.biWidth = Image->cols;
//	bmpInfo.bmiHeader.biPlanes = 1;
//	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	// Pointer to access the pixels of bitmap
//	BYTE* pPixels = 0, * pP;
//	hbmp = CreateDIBSection(NULL, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
//
//	if (!hbmp)
//		return hbmp; // return if invalid bitmaps
//
//	   //SetBitmapBits( hBitmap, lBmpSize, pBits);
//	   // Directly Write
//	int left_width = ((bpp * Image->cols + 31) / 32) * 4;
//	pP = pPixels;
//	for (int y = Image->rows - 1, row = 0; row < Image->rows; row++, y--) {
//		for (int x = 0, col = 0; col < Image->cols; x += 3, col++) {
//			pP[x] = tmp.at<Vec3b>(y, col).val[0];
//			pP[x + 1] = tmp.at<Vec3b>(y, col).val[1];
//			pP[x + 2] = tmp.at<Vec3b>(y, col).val[2];
//		}
//		pP += left_width;
//	}
//	return hbmp;
//}

HBITMAP CMFCwebnautesDlg::mat2bmp(cv::Mat* image)
{
	// 현재 응용프로그램의 스크린과 호환되는 memory dc를 생성한다.
	HDC         hDC = ::CreateCompatibleDC(0);
	BYTE        tmp[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	BITMAPINFO* bmi = (BITMAPINFO*)tmp;
	HBITMAP     hBmp;
	int i;
	int w = image->cols, h = image->rows;
	int bpp = image->channels() * 8;

	memset(bmi, 0, sizeof(BITMAPINFO));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = w;
	bmi->bmiHeader.biHeight = h;
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = bpp;
	bmi->bmiHeader.biCompression = BI_RGB;
	bmi->bmiHeader.biSizeImage = w * h * 1;
	bmi->bmiHeader.biClrImportant = 0;

	switch (bpp)
	{
	case 8:
		for (i = 0; i < 256; i++)
		{
			bmi->bmiColors[i].rgbBlue = i;
			bmi->bmiColors[i].rgbGreen = i;
			bmi->bmiColors[i].rgbRed = i;
		}
		break;
	case 32:
	case 24:
		((DWORD*)bmi->bmiColors)[0] = 0x00FF0000; /* red mask  */
		((DWORD*)bmi->bmiColors)[1] = 0x0000FF00; /* green mask */
		((DWORD*)bmi->bmiColors)[2] = 0x000000FF; /* blue mask  */
		break;
	}

	hBmp = ::CreateDIBSection(hDC, bmi, DIB_RGB_COLORS, NULL, 0, 0);
	::SetBitmapBits(hBmp, image->total() * image->channels(), image->data);
	::DeleteDC(hDC);

	return hBmp;
}

void CMFCwebnautesDlg::AddThumbnailList(CString FileName, Mat image) {
	// thumbnail 만들기
	Mat thumb = Mat(Size(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT), CV_8UC3);
		//cvCreateImage(cvSize(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT), IPL_DEPTH_8U, 3);
	resize(image, thumb, Size(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT), 0, 0, INTER_LINEAR);

	// 리스트에 추가
	InsertList(thumb, FileName);

	// thumbnail release
	if (!(thumb.empty())) thumb.release();
	//if (thumb != NULL) cvReleaseImage(&thumb);
}

void CMFCwebnautesDlg::InsertList(Mat thumb, CString FileName) {
	UpdateData(TRUE);
	LVITEM Item;

	// image list 길이 정의
	mImageList.SetImageCount(nIndex + 1);

	// 리스트에 아이템 추가
	Item.mask = LVIF_IMAGE | LVIF_TEXT;
	Item.iItem = nIndex;
	Item.iSubItem = 0;
	Item.pszText = FileName.GetBuffer(FileName.GetLength());
	Item.iImage = nIndex;
	m_piclist.InsertItem(&Item);

	// thumbnail 그리기
	CBitmap ThumbBitmap;
	HBITMAP bm = mat2bmp(&thumb);
	//HBITMAP bm = iplimage2hbitmap(thumb);
	ThumbBitmap.Attach(bm);
	mImageList.Replace(nIndex, &ThumbBitmap, NULL);
	m_piclist.RedrawItems(nIndex, nIndex);

	// bm release
	DeleteObject(bm);

	nIndex++;
}

void CMFCwebnautesDlg::removeAllImages() {
	// 캡쳐 파일 찾기
	CFileFind finder;
	CString strWildCard(CapPath);
	strWildCard += "\\";
	strWildCard += currentDate();
	strWildCard += "\\image_*.jpg";

	BOOL bWorking = finder.FindFile(strWildCard);
	while (bWorking) {
		bWorking = finder.FindNextFile();

		if (finder.IsDots() || finder.IsDirectory())
			continue;
		else {
			// 찾았으면 삭제
			DeleteFile(CapPath + finder.GetFileName());
		}
	}
	finder.Close();

	ShowThumbnailList();
}



void CMFCwebnautesDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMItemActivate->iItem != -1) {
		// 그림파일 열기
		CString tempPath = (CapPath + "\\" + currentDate() + "\\");
		CString strFullPath = tempPath + m_piclist.GetItemText(pNMItemActivate->iItem, 0);

		CImage tempImg;
		CWnd* pWnd = (CWnd*)GetDlgItem(IDC_PICTURE);
		CDC* dc = pWnd->GetDC();
		CStatic* staticSize = (CStatic*)GetDlgItem(IDC_PICTURE);
		CRect rect;

		staticSize->GetClientRect(rect);
		tempImg.Load(strFullPath);
		tempImg.Draw(dc->m_hDC, 0, 0, tempImg.GetWidth(), tempImg.GetHeight());
		
	}

	*pResult = 0;
}
