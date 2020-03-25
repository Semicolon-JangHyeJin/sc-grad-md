
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

using namespace cv;
using namespace std;

VideoCapture* capture;
Mat mat_frame;
CImage cimage_mfc;
//int insert = 0, v =0;
//Mat frame, gray, frameDelta, thresh, firstFrame;
//vector<vector<Point> > cnts;
//VideoCapture* camera;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//
//std::string currentDateTime() {
//	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
//	struct tm  tstruct;
//	char       buf[80];
//	tstruct = *localtime(&now);
//	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��
//	return buf;
//}
//
//void capPicture(Mat image) {
//	if (insert == 0) {
//		//char savefile[200];
//		string savefile = "image_" + currentDateTime() + ".jpg";
//		//sprintf(savefile, "image_%d.jpg", currentDateTime());
//		printf("cap %s\n", savefile);
//		imwrite(savefile, image);        // img�� ���Ϸ� �����Ѵ�.
//		insert++;
//		putText(image, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//
//	}
//	else if (insert > 3) {
//		//char savefile[200];
//		string savefile = "image_" + currentDateTime() + ".jpg";
//		//sprintf(savefile, "image_%d.jpg", currentDateTime());
//		imwrite(savefile, image);        // img�� ���Ϸ� �����Ѵ�.
//		printf("cap %s\n", savefile);
//		insert = 0;
//		putText(image, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//	}
//	else {
//		insert++;
//	}
//
//}

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
}

BEGIN_MESSAGE_MAP(CMFCwebnautesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_PICTURE, &CMFCwebnautesDlg::OnStnClickedPicture)
	ON_WM_DESTROY()
	ON_WM_TIMER()
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

	capture = new VideoCapture(1);
	if (!capture->isOpened())
	{
		MessageBox(_T("캠을 열수 없습니다. \n"));
	}

	//웹캠 크기를  320x240으로 지정    
	capture->set(CAP_PROP_FRAME_WIDTH, 320);
	capture->set(CAP_PROP_FRAME_HEIGHT, 240);

	SetTimer(1000, 30, NULL);
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


	//camera->read(frame);
	////convert to grayscale and set the first frame
	//cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
	//GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

	//while (camera->read(frame)) {

	//	//convert to grayscale
	//	cvtColor(frame, gray, COLOR_BGR2GRAY);
	//	GaussianBlur(gray, gray, Size(21, 21), 0);

	//	//compute difference between first frame and current frame
	//	absdiff(firstFrame, gray, frameDelta);
	//	threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);

	//	dilate(thresh, thresh, Mat(), Point(-1, -1), 2);
	//	findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//	for (int i = 0; i < cnts.size(); i++) {
	//		if (contourArea(cnts[i]) < 1500) {
	//			continue;
	//		}
	//		capPicture(frame);
	//	}

	//	imshow("Camera", frame);
	//	//camera.read(frame);
	//	Sleep(10);
	//	//convert to grayscale and set the first frame
	//	//v++;
	//	//if (v > 10) {
	//	cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
	//	GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
	//	//v = 0;
	////}
	//	if (waitKey(1) == 27) {
	//		//exit if ESC is pressed
	//		break;
	//	}

	//}

	CDialogEx::OnTimer(nIDEvent);
}

