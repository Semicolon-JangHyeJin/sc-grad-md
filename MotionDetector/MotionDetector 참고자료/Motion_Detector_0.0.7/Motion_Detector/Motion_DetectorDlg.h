
// Motion_DetectorDlg.h : ��� ����
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

#define _DEF_WEBCAM			1000	// CAM �� Timer ID
#define	THUMBNAIL_WIDTH		100		// thubmail �̹��� ũ��
#define	THUMBNAIL_HEIGHT	75		// thubmail �̹��� ũ��
#define SAVED_WIDTH			640
#define SAVED_HEIGHT		480
#define INI_CATEGORY		L"Motion_Detector"
#define INI_PARAMETER_DIFF	L"Diff_LEVEL"
#define INI_PARAMETER_PATH	L"Path"
#define INI_PARAMETER_PORT	L"Port"
#define INI_PARAMETER_DUEDATE	L"Due_Date"
#define INI_PARAMETER_COMBOSEL	L"Combo_Sel"
#define LEARN_FRAME			500		// Learning �ϴ� �� ��ٸ��� ���� ��ŵ�ϴ� ������ ��

// CMotion_DetectorDlg ��ȭ ����
class CMotion_DetectorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMotion_DetectorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MOTION_DETECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	ImageStreamingServer *pISS;
	
	CStatic mDisplay;			// ������ ������ static control
	CListCtrl mList;			// ���� ����Ʈ ������ list control
	CSliderCtrl mSlider;		// �ΰ��� �����ϴ� slider control
	CImageList mImageList;		// ���� ����Ʈ�� ���� ����� image list

	Motion_Detector_ConfigDlg *mMotion_Detector_ConfigDlg; // config dialog

	CvvImage	mCvvImage;		// �������� ����ϱ� ���� CvvImage

	CString	mPath;				// ������ ����Ǵ� path
	TCHAR iniPath[MAX_PATH];	// ini�� ����Ǵ� path
	UINT nIndex;				// ���� ����Ʈ�� ���Ǵ� list control �� image list �� index
	UINT mDiffLevel;			// �ΰ��� ����
	UINT mPort;					// ���� ��Ʈ ��ȣ
	UINT mComboSel;				// �޺� �ڽ�
	UINT mComboList2Date[14];	// ��¥�� ��ȯ ���̺�
	UINT mDueDate;				// ���� �� ��
	UINT mToday;				// ���� ��¥
	
	UINT skip_frame_cnt;		// skip�� ������ ��
	UINT cobject_cnt;			// ���� frame object ����
	UINT bobject_cnt;			// ���� frame object ����

public:
	void SaveDetectImage();										// ���� ����
	void InsertList(IplImage* thumb, CString FileName);			// ����Ʈ�� thumbnail �ֱ�
	void ShowThumbnailList();									// list control �� ���� thumbnail ������
	void AddThumbnailList(CString FileName, IplImage* image);	// list control �� thumbnail �߰�
	void removeAllImages();										// mPath �� ����� ��� ���� ����
	int GetFreeSpaceOfDrive(CString strVolDrive);				// ����̺� free space ����(MB��)

	afx_msg void OnDestroy();									// dialog �� ����� ��
	afx_msg void OnTimer(UINT_PTR nIDEvent);					// Timer �� ����� ��

	afx_msg void OnBnClickedBtnStart();							// ���� ��ư�� ������ ��
	afx_msg void OnBnClickedBtnStop();							// ���� ��ư�� ������ ��
	afx_msg void OnNMDblclkCaptureList(NMHDR *pNMHDR, LRESULT *pResult);	// list control ���� thumbnail�� ����Ŭ�� ���� ��
	afx_msg void OnNMReleasedcaptureDiffSlider(NMHDR *pNMHDR, LRESULT *pResult); // slider contorl �� Ŭ���ϰ� ���� ��
	afx_msg void OnEnChangeDiffRatioEdit();						// edit control �ٲ��� ��
	afx_msg void OnBnClickedBtnConfig();						// ��� ���� ��ư ������ ��
	afx_msg LRESULT OnSaveMessage(WPARAM wParam, LPARAM lParam);// config dialog���� message�� �޾��� ��
	afx_msg void OnSize(UINT nType, int cx, int cy);			// dialog size �� �ٲ� ��
};
