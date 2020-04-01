#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "GMM.h"
#include "BBox.h"

class ImageManager
{
public:
	ImageManager();
	virtual ~ImageManager();

public:
	GMM *mGMM;					// Gaussian Mixture Model ��ü
	BBox *mBBox;				// Binding Box ��ü

	CvCapture*	mCapture;		// Capture

	IplImage*	currentImage;	// ���� �̹���
	IplImage*	beforeImage;	// 1������ �� �̹���
	IplImage*	resultImage;	// �׽�Ʈ�� ����ϴ� ������ ��� �̹���

	BOOL isRunning;				// ���۵Ǿ��� �� ����

public:
	BOOL Start();				// imagemanager ����
	void Stop();				// imagemanager ����
	int GrabFrame(int mDiffLevel);			// ���� ������ ���
	void CopyCurrentToBefore();	// currentImage�� beforeImage�� ����
};