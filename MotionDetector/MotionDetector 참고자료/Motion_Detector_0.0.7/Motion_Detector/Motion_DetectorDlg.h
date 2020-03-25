
// Motion_DetectorDlg.h : 헤더 파일
//

#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "afxwin.h"
#include "afxcmn.h"
#include "CvvImage.h"
#include "DetectMotion.h"
#include "ImageStreamingServer.h"
#include "Motion_Detector_ConfigDlg.h"

#define _DEF_WEBCAM			1000	// CAM 용 Timer ID
#define	THUMBNAIL_WIDTH		100		// thubmail 이미지 크기
#define	THUMBNAIL_HEIGHT	75		// thubmail 이미지 크기
#define SAVED_WIDTH			640
#define SAVED_HEIGHT		480
#define INI_CATEGORY		L"Motion_Detector"
#define INI_PARAMETER_DIFF	L"Diff_LEVEL"
#define INI_PARAMETER_PATH	L"Path"
#define INI_PARAMETER_PORT	L"Port"
#define INI_PARAMETER_DUEDATE	L"Due_Date"
#define INI_PARAMETER_COMBOSEL	L"Combo_Sel"
#define LEARN_FRAME			500		// Learning 하는 걸 기다리기 위해 스킵하는 프래임 수

// CMotion_DetectorDlg 대화 상자
class CMotion_DetectorDlg : public CDialogEx
{
// 생성입니다.
public:
	CMotion_DetectorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTION_DETECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	ImageStreamingServer *pISS;
	
	CStatic mDisplay;			// 동영상 나오는 static control
	CListCtrl mList;			// 사진 리스트 나오는 list control
	CSliderCtrl mSlider;		// 민감도 조정하는 slider control
	CImageList mImageList;		// 사진 리스트에 같에 사용할 image list

	Motion_Detector_ConfigDlg *mMotion_Detector_ConfigDlg; // config dialog

	CvvImage	mCvvImage;		// 동영상을 출력하기 위한 CvvImage

	CString	mPath;				// 사진이 저장되는 path
	TCHAR iniPath[MAX_PATH];	// ini가 저장되는 path
	UINT nIndex;				// 사진 리스트에 사용되는 list control 과 image list 의 index
	UINT mDiffLevel;			// 민감도 레벨
	UINT mPort;					// 서버 포트 번호
	UINT mComboSel;				// 콤보 박스
	UINT mComboList2Date[14];	// 날짜별 변환 테이블
	UINT mDueDate;				// 남은 일 수
	UINT mToday;				// 오늘 날짜
	
	UINT skip_frame_cnt;		// skip할 프레임 수
	UINT cobject_cnt;			// 현재 frame object 갯수
	UINT bobject_cnt;			// 이전 frame object 갯수

public:
	void SaveDetectImage();										// 사진 저장
	void InsertList(IplImage* thumb, CString FileName);			// 리스트에 thumbnail 넣기
	void ShowThumbnailList();									// list control 에 사진 thumbnail 보여줌
	void AddThumbnailList(CString FileName, IplImage* image);	// list control 에 thumbnail 추가
	void removeAllImages();										// mPath 에 저장된 모든 사진 삭제
	int GetFreeSpaceOfDrive(CString strVolDrive);				// 드라이브 free space 구함(MB로)

	afx_msg void OnDestroy();									// dialog 가 사라질 때
	afx_msg void OnTimer(UINT_PTR nIDEvent);					// Timer 가 실행될 때

	afx_msg void OnBnClickedBtnStart();							// 시작 버튼을 눌렀을 때
	afx_msg void OnBnClickedBtnStop();							// 정지 버튼을 눌렀을 때
	afx_msg void OnNMDblclkCaptureList(NMHDR *pNMHDR, LRESULT *pResult);	// list control 에서 thumbnail을 더블클릭 했을 때
	afx_msg void OnNMReleasedcaptureDiffSlider(NMHDR *pNMHDR, LRESULT *pResult); // slider contorl 을 클릭하고 뗏을 때
	afx_msg void OnEnChangeDiffRatioEdit();						// edit control 바꿨을 때
	afx_msg void OnBnClickedBtnConfig();						// 고급 설정 버튼 눌렀을 때
	afx_msg LRESULT OnSaveMessage(WPARAM wParam, LPARAM lParam);// config dialog에서 message를 받았을 때
	afx_msg void OnSize(UINT nType, int cx, int cy);			// dialog size 를 바꿀 때
};
